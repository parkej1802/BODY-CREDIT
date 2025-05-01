// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNox_EBase.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

ACNox_EBase::ACNox_EBase()
{
	TeamID = 2;
	
	Perception = CreateDefaultSubobject<UAIPerceptionComponent>("Perception");

	// Sight
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = SightRadius;
	Sight->LoseSightRadius = LoseSightRadius;
	Sight->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
	Sight->SetMaxAge(RetentionTime);

	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = false;
	Sight->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Sight);

	// Hearing
	Hearing = CreateDefaultSubobject<UAISenseConfig_Hearing>("Hearing");
	Hearing->HearingRange = HearingRange;
	Hearing->SetMaxAge(RetentionTime);
	Hearing->DetectionByAffiliation.bDetectEnemies = true;
	Hearing->DetectionByAffiliation.bDetectNeutrals = false;
	Hearing->DetectionByAffiliation.bDetectFriendlies = false;

	Perception->ConfigureSense(*Hearing);

	Perception->SetDominantSense(*Sight->GetSenseImplementation());	
}
