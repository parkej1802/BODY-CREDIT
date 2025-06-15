#include "Games/CMainGM.h"

#include "EngineUtils.h"
#include "Global.h"
#include "Characters/CNox_Runner.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"
#include "Trigger/CAreaTriggerBox.h"
#include "Lobby/LobbyWidget_Failed.h"
#include "Characters/CNox_Controller.h"
#include "Trigger/CSpawnBoundaryBox.h"
#include "GameFramework/PlayerStart.h"

ACMainGM::ACMainGM()
{
	PrimaryActorTick.bCanEverTick = true;

	/*ConstructorHelpers::FClassFinder<APawn> pawn(
		TEXT("/Game/Characters/Runner/BP_CNox_Runner.BP_CNox_Runner_C"));
	if (pawn.Succeeded())
		DefaultPawnClass = pawn.Class;*/


}

void ACMainGM::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<ACNox_Controller>(GetWorld()->GetFirstPlayerController());
	PlayerCharacter = Cast<ACNox_Runner>(PC->GetPawn());

	for (TActorIterator<ACNox_MemoryCollectorAI> It(GetWorld(), ACNox_MemoryCollectorAI::StaticClass()); It; ++It)
	{
		MemoryCollectorAI = *It;
	}
	for (TActorIterator<ACAreaTriggerBox> It(GetWorld(), ACAreaTriggerBox::StaticClass()); It; ++It)
	{
		ZoneVolumes.Add(*It);
	}
}

void ACMainGM::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsStart) {
		if (GameTimer > 0.f)
		{
			GameTimer -= DeltaSeconds;
			GameTimer = FMath::Max(0.f, GameTimer);
		}
		else if (!bIsFailed)
		{
			bIsFailed = true;

			if (FailedWidgetClass)
			{
				if (PC)
				{
					ULobbyWidget_Failed* FailedWidget = CreateWidget<ULobbyWidget_Failed>(PC, FailedWidgetClass);
					if (FailedWidget)
					{
						FailedWidget->AddToViewport();

						FInputModeUIOnly InputMode;
						InputMode.SetWidgetToFocus(FailedWidget->TakeWidget());
						PC->SetInputMode(InputMode);
						PC->bShowMouseCursor = true;
					}

					for (TActorIterator<ACNox_EBase> It(GetWorld()); It; ++It)
					{
						(*It)->DayStart();
					}
				}
			}
		}
	}
}

AActor* ACMainGM::ChoosePlayerStart_Implementation(AController* Player)
{
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (It->PlayerStartTag == FName(TEXT("Lobby")))
		{
			return *It;
		}
	}

	// 그 이후(또는 'Initial' 못 찾음)엔 기본 로직 사용
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ACMainGM::RegisterMemoryFromPlayer(ACNox_Runner* Player, EMemoryTriggerType Trigger)
{
	if (!ExtractTimerTriggerStart && !Player) return;
	if (FMath::FRandRange(0.0, 1.0) > RegisterPercent) return;

	FMemoryFragment NewMemory;
	NewMemory.Location = Player->GetActorLocation();
	NewMemory.TimeStamp = GetWorld()->GetTimeSeconds();
	NewMemory.TriggerType = Trigger;
	NewMemory.SourcePlayer = Player;
	NewMemory.ZoneID = GetZoneID(Player);
	NewMemory.bVIPRelated = IsInVIPZone(NewMemory.ZoneID);

	if (!MemoryCollectorAI) return;
	MemoryCollectorAI->RegisterMemory(NewMemory); // AI에 직접 등록
}

int32 ACMainGM::GetItemIndex()
{
	return ++ItemIndex;
}

FName ACMainGM::GetZoneID(class ACNox_Runner* Player)
{
	if (!Player) return NAME_None;

	for (const auto& Zone : ZoneVolumes)
	{
		if (Zone && Zone->IsOverlappingActor(Player))
		{
			return Zone->ZoneID; // ex. "VIP_Ward", "EdenWing"
		}
	}
	return NAME_None;
}

bool ACMainGM::IsInVIPZone(const FName& ZoneID)
{
	return ZoneID.ToString().Equals(VIPZoneID);
}

float ACMainGM::GetGamePlayTime()
{
	if (ExtractTimerTriggerStart) GameStartTime+=GetWorld()->DeltaTimeSeconds;
	else GameStartTime = 0.f;

	return GameStartTime;
}

void ACMainGM::SpawnEnemy()
{
	if (SpawnBoundaryArray.Num()==0)
	{
		for (TActorIterator<ACSpawnBoundaryBox> It(GetWorld()); It; ++It)
		{
			SpawnBoundaryArray.Emplace(*It);
		}		
	}

	if (SpawnBoundaryArray.Num()==0) return;
	
	{ // Zero
		int32 spawnCnt = 0;
		while (spawnCnt++ < SpawnData.ZeroSpawnCount)
		{
			SpawnEnemy(ZeroFactory, GetSpawnRandomLoc(GetSpawnBoundaryBox(1)));
		}
	}

	{ // medic
		int32 spawnCnt = 0;
		while (spawnCnt++ < SpawnData.MedicSpawnCount)
		{
			SpawnEnemy(MedicFactory, GetSpawnRandomLoc(GetSpawnBoundaryBox(2)));
		}
	}

	{ // memory
		int32 spawnCnt = 0;
		while (spawnCnt++ < SpawnData.MemorySpawnCount)
		{
			SpawnEnemy(MemoryFactory, GetSpawnRandomLoc(GetSpawnBoundaryBox(2)));
		}
	}
}

ACSpawnBoundaryBox* ACMainGM::GetSpawnBoundaryBox(const int32 SpawnMinFloor)
{
	ACSpawnBoundaryBox* Boundary=nullptr;
	while (true)
	{
		if (Boundary && Boundary->GetFloor()>=SpawnMinFloor) return Boundary;

		int32 RandomIndex = FMath::RandRange(0, SpawnBoundaryArray.Num() - 1);
		Boundary = SpawnBoundaryArray[RandomIndex];
	}
}

FVector ACMainGM::GetSpawnRandomLoc(const ACSpawnBoundaryBox* SpawnBoundaryBox)
{
	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnBoundaryBox->GetLoc(), SpawnBoundaryBox->GetScale());
}

void ACMainGM::SpawnEnemy(const TSubclassOf<ACNox_EBase>& SpawnCls, const FVector& SpawnLoc) const
{
	if (!SpawnCls) return;
	FVector SpawnPos = SpawnLoc + FVector(0, 0, 100);
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	GetWorld()->SpawnActor<ACNox_EBase>(SpawnCls, SpawnPos,FRotator::ZeroRotator, params);
}

void ACMainGM::DestroyEnemy()
{
	ExtractTimerTriggerStart = false;
	
	for (TActorIterator<ACNox_EBase> It(GetWorld()); It; ++It)
	{
		(*It)->Destroy();
	}
}

void ACMainGM::ChangePlayerStartLocation()
{
	APlayerStart* Start = nullptr;
	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (It->PlayerStartTag == "Start")
		{
			Start = *It;

			break;
		}
	}
		
	PlayerCharacter->SetActorLocation(Start->GetActorLocation());
	PlayerCharacter->GetController()->SetControlRotation(Start->GetActorRotation());
}

// bool ACMainGM::IsInVIPZone(ACNox_Runner* Player)
// {
// 	if (!Player) return false;
//
// 	// 예시: VIPZoneVolume은 레벨에 배치된 TriggerVolume이나 BoxComponent 기반의 Actor
// 	for (const auto& Zone : ZoneVolumes)
// 	{
// 		if (Zone && Zone->ActorHasTag(FName("VIP")) && Zone->IsOverlappingActor(Player))
// 		{
// 			return true;
// 		}
// 	}
// 	return false;
// }
