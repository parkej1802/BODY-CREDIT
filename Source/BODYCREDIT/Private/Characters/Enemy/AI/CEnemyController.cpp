// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/CEnemyController.h"

#include "global.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_CCTV.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Components/Enemy/CNox_BehaviorComponent.h"
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

void ACEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	EnemyBase = Cast<ACNox_EBase>(InPawn);
	if (EnemyBase)
		SetGenericTeamId(EnemyBase->TeamID);

	if (EnemyBase->bUseBehaviorTree)
	{
		BT_Behavior = CHelpers::GetComponent<UCNox_BehaviorComponent>(EnemyBase);

		check(EnemyBase->GetBehaviorTree());

		UBlackboardComponent* blackboard = Blackboard.Get();
		if (UseBlackboard(EnemyBase->GetBehaviorTree()->BlackboardAsset, blackboard))
			this->Blackboard = blackboard;

		BT_Behavior->SetBlackboard(Blackboard);

		RunBehaviorTree(NoxBehaviorTree);
	}

	InitPerception();
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACEnemyController::OnPerceptionUpdated);
	Perception->OnTargetPerceptionInfoUpdated.AddDynamic(this, &ACEnemyController::OnAITargetPerceptionInfoUpdate);
}

void ACEnemyController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	if (TargetPlayer) return;

	// UpdatedActors는 모든 변경 사항에 대해 적용되어 추가된건지 제외된건지 알기 어렵다.
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);
	for (auto& actor : actors)
	{
		// CLog::Log(actor);
	}
}

void ACEnemyController::OnAITargetPerceptionInfoUpdate(const FActorPerceptionUpdateInfo& UpdateInfo)
{
	if (UpdateInfo.Stimulus.WasSuccessfullySensed())
	{
		if (TargetPlayer) return;
		bExpiredStimuli = false;
		// 적이 갱신되거나 재감지되었을 때
		CLog::Log(FString::Printf(TEXT("Actor Sensed: %s"), *UpdateInfo.Target.Get()->GetName()));

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
					TargetPlayer = nox;
				}
			}
		}

		EnemyBase->SetTarget(TargetPlayer);
	}
	else
	{
		if (!TargetPlayer) return;
		if (TargetPlayer != UpdateInfo.Target.Get()) return;

		bExpiredStimuli = true;
		// 적이 잊혀졌을 때
		CLog::Log(FString::Printf(TEXT("Actor Forgotten: %s"), *UpdateInfo.Target.Get()->GetName()));
	}
}

void ACEnemyController::UpdateExpiredStimuli(float DeltaTime)
{
	if (!bExpiredStimuli) return;
	if (!TargetPlayer) return;
	if (EnemyBase->GetRetentionTime() == KINDA_SMALL_NUMBER) return;

	CurExpiredTime += DeltaTime;
	if (CurExpiredTime >= Sight->GetMaxAge())
	{
		bExpiredStimuli = false;
		TargetPlayer = nullptr;
		CurExpiredTime = 0.f;
		EnemyBase->SetTarget(TargetPlayer);
	}
}
