#include "Characters/Enemy/AI/CEnemyController.h"
#include "Global.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

#pragma region Init
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

void ACEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	EnemyBase = Cast<ACNox_EBase>(InPawn);
	if (EnemyBase) SetGenericTeamId(EnemyBase->TeamID); // TeamID 설정

	InitPerception(); // Perception Settings
	Perception->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ACEnemyController::OnAITargetPerceptionInfoUpdate);
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

void ACEnemyController::OnAITargetPerceptionInfoUpdate(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	// Sencing 타입에 따라 분기처리
	auto StimulusType = UpdateInfo.Stimulus.Tag;
	if (StimulusType.ToString().Equals("Hearing"))
	{
		// 청각
		EnemyBase->bHearingMovement = true;
		EnemyBase->HearingLoc = UpdateInfo.Stimulus.StimulusLocation;
		return;
	}

	// 시각
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
#pragma endregion

#pragma region Get Near Player
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
#pragma endregion

#pragma region Tick
void ACEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateExpiredStimuli(DeltaSeconds);
}
#pragma endregion

#pragma region Set Target
void ACEnemyController::SetTargetPlayer(ACNox* InTargetPlayer)
{
	TargetPlayer = InTargetPlayer;
}
#pragma endregion

#pragma region Target loss
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
#pragma endregion

#pragma region Stop Perception (Using Die)
void ACEnemyController::PerceptionDeactive()
{
	StopMovement();
	Perception->OnTargetPerceptionInfoUpdated.Clear();
}
#pragma endregion
