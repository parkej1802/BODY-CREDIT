// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNox_EBase.h"

#include "global.h"
#include "Characters/Enemy/AI/CEnemyController.h"

ACNox_EBase::ACNox_EBase()
{
	TeamID = 2;

	// Controller Setting
	AIControllerClass = ACEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ACNox_EBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACNox_EBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
