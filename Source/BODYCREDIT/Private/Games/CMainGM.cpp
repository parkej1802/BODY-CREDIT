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

	// ConstructorHelpers::FClassFinder<APawn> pawn(
	// 	TEXT("/Script/Engine.Blueprint'/Game/Characters/Runner/BP_CNox_Runner.BP_CNox_Runner_C'"));
	// if (pawn.Succeeded())
	// 	DefaultPawnClass = pawn.Class;



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

						FInputModeGameAndUI InputMode;
						InputMode.SetWidgetToFocus(FailedWidget->TakeWidget());
						PC->SetInputMode(InputMode);
						PC->bShowMouseCursor = true;
					}
				}
			}
		}
	}

	if (ExtractTimerTriggerStart) GameStartTime += DeltaSeconds;
	else GameStartTime = 0.f;
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
	return GameStartTime;
}

void ACMainGM::SpawnEnemy()
{
	if (bSpawnEnemy) return;
	bSpawnEnemy = true;
	
	if (SpawnBoundaryCount==0)
	{
		for (TActorIterator<ACSpawnBoundaryBox> It(GetWorld()); It; ++It)
		{
			if ((*It)->GetSpawnAbleZeroCount() > 0) ZeroSpawnBoundaryArray.Emplace(*It);
			if ((*It)->GetSpawnAbleMedicCount() > 0) MedicSpawnBoundaryArray.Emplace(*It);
			if ((*It)->GetSpawnAbleMemoryCount() > 0) MemorySpawnBoundaryArray.Emplace(*It);
			SpawnAbleZeroCount += (*It)->GetSpawnAbleZeroCount();
			SpawnAbleMedicCount += (*It)->GetSpawnAbleMedicCount();
			SpawnAbleMemoryCount += (*It)->GetSpawnAbleMemoryCount();
			++SpawnBoundaryCount;
		}

		SpawnData.ZeroSpawnCount = FMath::Min(SpawnAbleZeroCount, SpawnData.ZeroSpawnCount);
		SpawnData.MedicSpawnCount = FMath::Min(SpawnAbleMedicCount, SpawnData.MedicSpawnCount);
		SpawnData.MemorySpawnCount = FMath::Min(SpawnAbleMemoryCount, SpawnData.MemorySpawnCount);
	}

	if (SpawnBoundaryCount == 0) return;
	
	{ // Zero
		int32 spawnCnt = 0;
		while (spawnCnt < SpawnData.ZeroSpawnCount)
		{
			auto* SpawnBox = GetSpawnBoundaryBox(ZeroSpawnBoundaryArray,1);
			if (!SpawnBox) break;
			if (SpawnBox->CanSpawnZero())
			{
				SpawnEnemy(ZeroFactory, GetSpawnRandomLoc(SpawnBox));
				SpawnBox->AppendZeroCount();
				++spawnCnt;
			}
		}
	}

	{ // medic
		int32 spawnCnt = 0;
		while (spawnCnt < SpawnData.MedicSpawnCount)
		{
			auto* SpawnBox = GetSpawnBoundaryBox(MedicSpawnBoundaryArray,2);
			if (!SpawnBox) break;
			if (SpawnBox->CanSpawnMedic())
			{
				SpawnEnemy(MedicFactory, GetSpawnRandomLoc(SpawnBox));
				SpawnBox->AppendMedicCount();
				++spawnCnt;
			}
			else break;
		}
	}

	{ // memory
		int32 spawnCnt = 0;
		while (spawnCnt < SpawnData.MemorySpawnCount)
		{
			auto* SpawnBox = GetSpawnBoundaryBox(MemorySpawnBoundaryArray,2);
			if (!SpawnBox) break;
			if (SpawnBox->CanSpawnMemory())
			{
				SpawnEnemy(MemoryFactory, GetSpawnRandomLoc(SpawnBox));
				SpawnBox->AppendMemoryCount();
				++spawnCnt;
			}
		}
	}
}

ACSpawnBoundaryBox* ACMainGM::GetSpawnBoundaryBox(const TArray<ACSpawnBoundaryBox*>& SpawnBoundaryBox, const int32 SpawnMinFloor)
{
	ACSpawnBoundaryBox* Boundary=nullptr;
	if (!SpawnBoundaryBoxCheck(SpawnBoundaryBox, SpawnMinFloor)) return nullptr;
	while (true)
	{
		if (Boundary && Boundary->GetFloor()>=SpawnMinFloor) return Boundary;

		int32 RandomIndex = FMath::RandRange(0, SpawnBoundaryBox.Num() - 1);
		Boundary = SpawnBoundaryBox[RandomIndex];
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

bool ACMainGM::SpawnBoundaryBoxCheck(const TArray<ACSpawnBoundaryBox*>& SpawnBoundaryBox, const int32 Floor)
{
	for (auto& Boundary : SpawnBoundaryBox)
	{
		if (Boundary->GetFloor() >= Floor) return true;
	}
	return false;
}

void ACMainGM::DestroyEnemy()
{
	ExtractTimerTriggerStart = false;
	bSpawnEnemy = false;
	
	for (TActorIterator<ACNox_EBase> It(GetWorld()); It; ++It)
	{
		(*It)->Destroy();
	}

	for (auto& Boundary : ZeroSpawnBoundaryArray)
	{
		Boundary->ResetCount();
	}
	for (auto& Boundary : MedicSpawnBoundaryArray)
	{
		Boundary->ResetCount();
	}
	for (auto& Boundary : MemorySpawnBoundaryArray)
	{
		Boundary->ResetCount();
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
