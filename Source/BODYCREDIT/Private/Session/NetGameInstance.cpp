﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "Session/NetGameInstance.h"
#include "BODYCREDIT.h"
#include "Characters/CNox_Controller.h"
#include "Characters/CNox_Runner.h"

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

