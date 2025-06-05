#include "Characters/Enemy/AI/CEnemyController.h"
#include "Global.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

ACEnemyController::ACEnemyController()
{
	CHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	// Sight
	CHelpers::CreateActorComponent<UAISenseConfig_Sight>(this, &Sight, "Sight");
	Perception->ConfigureSense(*Sight);
	// Hearing
	CHelpers::CreateActorComponent<UAISenseConfig_Hearing>(this, &Hearing, "Hearing");
	Perception->ConfigureSense(*Hearing);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ACEnemyController::BeginPlay()
{
	Super::BeginPlay();
}

void ACEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateExpiredStimuli(DeltaSeconds);
}

void ACEnemyController::InitPerception()
{
	Sight->SightRadius = EnemyBase->GetSightRadius();
	Sight->LoseSightRadius = EnemyBase->GetLoseSightRadius();
	Sight->PeripheralVisionAngleDegrees = EnemyBase->GetPeripheralVisionAngleDegrees();
	Sight->SetMaxAge(EnemyBase->GetRetentionTime());
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Hearing->HearingRange = EnemyBase->GetHearingRange();
	Hearing->SetMaxAge(EnemyBase->GetRetentionTime());
	Hearing->DetectionByAffiliation.bDetectEnemies = true;
	Hearing->DetectionByAffiliation.bDetectNeutrals = false;
	Hearing->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->RequestStimuliListenerUpdate();
}

void ACEnemyController::SetTargetPlayer(ACNox* InTargetPlayer)
{
	TargetPlayer = InTargetPlayer;
}

void ACEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	EnemyBase = Cast<ACNox_EBase>(InPawn);
	if (EnemyBase) SetGenericTeamId(EnemyBase->TeamID);	// TeamID 설정

	InitPerception();	// Perception Settings
	Perception->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ACEnemyController::OnAITargetPerceptionInfoUpdate);
}

void ACEnemyController::OnAITargetPerceptionInfoUpdate(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if (UpdateInfo.Stimulus.WasSuccessfullySensed())
	{
		if (TargetPlayer) return;
		TargetPlayer = GetNearTargetPlayer();

		StopMovement();
		EnemyBase->SetTarget(TargetPlayer);

		// CCTV가 플레이어를 발견하면 주변의 Enemy에게 알린다
		if (TargetPlayer) OnDetectPlayer.ExecuteIfBound(TargetPlayer);
	}
}

// 가까운 플레이어 구하기 (멀티플레이 고려)
ACNox* ACEnemyController::GetNearTargetPlayer()
{
	ACNox* NearTarget = nullptr;
	float MinDistance = FLT_MAX;
	FVector MyLoc = EnemyBase->GetActorLocation();

	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);
	for (auto& actor : actors)
	{
		if (auto nox = Cast<ACNox>(actor))
		{
			float tmpDist = FVector::Dist(MyLoc, actor->GetActorLocation());
			if (tmpDist < MinDistance)
			{
				MinDistance = tmpDist;
				NearTarget = nox;
			}
		}
	}
	return NearTarget;
}

// 타겟 잃어버리기
void ACEnemyController::UpdateExpiredStimuli(float DeltaTime)
{
	if (!TargetPlayer) return;
	if (FVector::Dist(TargetPlayer->GetActorLocation(), EnemyBase->GetActorLocation()) < Sight->LoseSightRadius)
	{
		CurExpiredTime = 0.f;
		return;
	}
	if (EnemyBase->GetRetentionTime() == KINDA_SMALL_NUMBER) return;

	CurExpiredTime += DeltaTime;
	if (CurExpiredTime >= Sight->GetMaxAge())
	{
		TargetPlayer = nullptr;
		CurExpiredTime = 0.f;
		EnemyBase->SetTarget(TargetPlayer);
		StopMovement();
	}
}

void ACEnemyController::PerceptionDeactive()
{
	PerceptionComponent->Deactivate();
}
