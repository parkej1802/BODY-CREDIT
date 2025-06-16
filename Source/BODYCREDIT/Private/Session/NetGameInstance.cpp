#include "Session/NetGameInstance.h"
#include "BODYCREDIT.h"
#include "Characters/CNox_Controller.h"
#include "Characters/CNox_Runner.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundMix.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Item/ItemDT.h"
#include "Item/Item_Base.h"

void UNetGameInstance::Init()
{	
	Super::Init();

	ItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Item/DT_ItemData.DT_ItemData"));
	StageDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/GameState/StageDT.StageDT"));

	if (auto subsys = IOnlineSubsystem::Get())
	{
		sessionInterface = subsys->GetSessionInterface();

		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnCreateSessionComplete);

		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, 
			FTimerDelegate::CreateLambda([&]
				{
					CreateMySession(mySessionName, 10);
				}
			), 2.f, false);
	}

	AlivePart.Add(EPlayerPart::Head, true);
	AlivePart.Add(EPlayerPart::Body, true);
	AlivePart.Add(EPlayerPart::Arm, true);
	AlivePart.Add(EPlayerPart::Leg, true);

	InitItemCache(ItemDataTable);

	InitMix();
}

void UNetGameInstance::CreateMySession(FString roomName, int32 playerCount)
{
	// 세션설정 변수
	FOnlineSessionSettings sessionSettings;

	// 1. Dedicated Server 접속 여부
	sessionSettings.bIsDedicated = false;

	// 2. 랜선(로컬)매칭을 할지 steam 매칭을 할지 여부
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	sessionSettings.bIsLANMatch = (subsysName == "NULL");

	// 3. 매칭이 온라인을 통해 노출될지 여부
	// false 이면 초대를 통해서만 입장이 가능
	// SendSessionInviteToFriend() 함수를 통해 친구를 초대할 수 있다.
	sessionSettings.bShouldAdvertise = true;

	// 4. 온라인 상태(Presence) 정보를 활용할지 여부
	sessionSettings.bUsesPresence = true;
	sessionSettings.bUseLobbiesIfAvailable = true;

	// 5. 게임진행중에 참여 허가할지 여부
	sessionSettings.bAllowJoinViaPresence = true;
	sessionSettings.bAllowJoinInProgress = true;

	// 6. 세션에 참여할 수 있는 공개(public) 연결의 최대 허용 수
	sessionSettings.NumPublicConnections = playerCount;

	// 7. 커스텀 룸네임 설정
	sessionSettings.Set(FName("ROOM_NAME"), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 8. 호스트 네임 설정
	sessionSettings.Set(FName("HOST_NAME"), mySessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// netID
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	//PRINTLOG(TEXT("Create Session Strat : %s"), *mySessionName);
	sessionInterface->CreateSession(*netID, FName(mySessionName), sessionSettings);
}

void UNetGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	//PRINTLOG(TEXT("SessionName : %s, bWasSuccessful : %d"), *SessionName.ToString(), bWasSuccessful);
}

void UNetGameInstance::SetPlayerGold(int32 NewGold)
{
	PlayerGold = NewGold;
	OnGoldChanged.Broadcast(NewGold);
}

void UNetGameInstance::SetActorInitLocation()
{
	PC = GetWorld()->GetFirstPlayerController();
	APawn* Pawn = PC->GetPawn();

	PlayerCharacter = Cast<ACNox_Runner>(Pawn);

	FVector StartLocation(10000.f, 0.f, 100.f);
	PlayerCharacter->SetActorLocation(StartLocation);
}

void UNetGameInstance::RefreshGame()
{
	PayTime = false;
	Failed = false;
	AmountToPay = 0;
	DayLeft = -1;
	RemainingLife = 4;
	SelectedPart = EPlayerPart::Basic;
	AlivePart[EPlayerPart::Head] = true;
	AlivePart[EPlayerPart::Body] = true;
	AlivePart[EPlayerPart::Arm] = true;
	AlivePart[EPlayerPart::Leg] = true;
	BeforePlayerGold = 0;
	AfterPlayerGold = 0;
	// PlayerGold = InitialPlayerGold;
	Debt = 10000000;
}

void UNetGameInstance::PostInitProperties()
{
	Super::PostInitProperties();

	InitialPlayerGold = PlayerGold;
}

EItemRarity UNetGameInstance::GetRandomRarity()
{
	float Rand = FMath::FRand();
	float Accum = 0.f;

	if (Rand < (Accum += RarityRate.Common)) return EItemRarity::Common;
	if (Rand < (Accum += RarityRate.Rare)) return EItemRarity::Rare;
	if (Rand < (Accum += RarityRate.Epic)) return EItemRarity::Epic;
	if (Rand < (Accum += RarityRate.Legendary)) return EItemRarity::Legendary;

	return EItemRarity::Blank;
}

EItemRarity UNetGameInstance::GetRandomRarityByLootTier(ELootBoxTier Tier)
{
	FRarityProbability* Prob = LootBoxRarityMap.Find(Tier);
	if (!Prob) return EItemRarity::Common;

	float Roll = FMath::FRand();
	float Accum = 0.f;

	if (Roll < (Accum += Prob->Common))     return EItemRarity::Common;
	if (Roll < (Accum += Prob->Rare))       return EItemRarity::Rare;
	if (Roll < (Accum += Prob->Epic))       return EItemRarity::Epic;
	if (Roll < (Accum += Prob->Legendary))  return EItemRarity::Legendary;

	return EItemRarity::Common;
}

void UNetGameInstance::InitMix()
{
	if (!BGM_Mix)
	{
		UE_LOG(LogTemp, Warning, TEXT("BGM_Mix is not set in GameInstance"));
		return;
	}

	// Sound Mix 활성화
    UGameplayStatics::SetBaseSoundMix(GetWorld(), BGM_Mix);
    UGameplayStatics::PushSoundMixModifier(GetWorld(), BGM_Mix);

	// 초기 볼륨 설정
	SetLobbySound(1.0f);
	SetInGameSound(1.0f);
	SetExtractSound(1.0f);
}

void UNetGameInstance::SetLobbySound(float Volume, float Pitch)
{
	if (!BGM_Mix || !LobbySoundClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("BGM_Mix or LobbySoundClass is not set in GameInstance"));
		return;
	}
	// LobbySoundClass->Properties.Volume = Volume;
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), BGM_Mix, LobbySoundClass, Volume, Pitch, 0.0f);
}

void UNetGameInstance::SetInGameSound(float Volume, float Pitch)
{
	if (!BGM_Mix || !InGameSoundClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("BGM_Mix or InGameSoundClass is not set in GameInstance"));
		return;
	}
 
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), BGM_Mix, InGameSoundClass, Volume, Pitch, 0.0f);
}

void UNetGameInstance::SetExtractSound(float Volume, float Pitch)
{
	if (!BGM_Mix || !ExtractSoundClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("BGM_Mix or ExtractSoundClass is not set in GameInstance"));
		return;
	}

	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), BGM_Mix, ExtractSoundClass, Volume, Pitch, 0.0f);
}

void UNetGameInstance::InitItemCache(UDataTable* ItemDT)
{
	if (!ItemDT) return;

	CachedItemLists = {};

	TArray<FName> RowNames = ItemDT->GetRowNames();

	for (const FName& RowName : RowNames)
	{
		FItemData* Row = ItemDT->FindRow<FItemData>(RowName, TEXT("Cache"));
		if (!Row || !Row->ItemClass) continue;
		if (Row->ItemType != EPlayerPart::Basic) continue;

		switch (Row->Rarity)
		{
		case EItemRarity::Common:    CachedItemLists.Common.Add(Row); break;
		case EItemRarity::Rare:      CachedItemLists.Rare.Add(Row); break;
		case EItemRarity::Epic:      CachedItemLists.Epic.Add(Row); break;
		case EItemRarity::Legendary: CachedItemLists.Legendary.Add(Row); break;
		default: break;
		}
	}
}

USoundCue* UNetGameInstance::GetBGM()
{
	return BGM_Cue;
}

void UNetGameInstance::PlayBGM(const int32 idx)
{
	if (ACNox_Controller* Controller = Cast<ACNox_Controller>(GetFirstLocalPlayerController()))
	{
		Controller->PlayBGM(idx);
	}
}

void UNetGameInstance::StopBGM()
{
	if (ACNox_Controller* Controller = Cast<ACNox_Controller>(GetFirstLocalPlayerController()))
	{
		Controller->StopBGM();
	}
}

void UNetGameInstance::PauseBGM(bool bIsPause)
{
	if (ACNox_Controller* Controller = Cast<ACNox_Controller>(GetFirstLocalPlayerController()))
	{
		Controller->PauseBGM(bIsPause);
	}
}

