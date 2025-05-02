// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/CEnemyController.h"

#include "global.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

ACEnemyController::ACEnemyController()
{
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

	// Sight
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Perception->ConfigureSense(*Sight);
	// Hearing
	Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>("Hearing");
	Perception->ConfigureSense(*Hearing);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());
}

void ACEnemyController::BeginPlay()
{
	Super::BeginPlay();
	InitPerception();
	Perception->OnPerceptionUpdated.AddDynamic(this, &ACEnemyController::OnPerceptionUpdated);
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
}

void ACEnemyController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);
	for (auto& actor : actors)
	{
		CLog::Log(actor);
	}
}