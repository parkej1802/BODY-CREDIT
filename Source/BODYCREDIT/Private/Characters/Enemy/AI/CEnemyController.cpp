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
	InitPerception();
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACEnemyController::OnPerceptionUpdated);
}

void ACEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
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
}

void ACEnemyController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);
	for (auto& actor : actors)
	{
		// CLog::Log(actor);
	}
}
