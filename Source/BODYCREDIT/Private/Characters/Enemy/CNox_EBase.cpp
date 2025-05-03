// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNox_EBase.h"

#include "global.h"
#include "Characters/Enemy/AI/CEnemyController.h"
#include "Components/Enemy/CNox_BehaviorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ACNox_EBase::ACNox_EBase()
{
	TeamID = 2;

	CHelpers::CreateActorComponent<UCNox_BehaviorComponent>(this, &BehaviorComp, "Behavior");

	// Controller Setting
	CHelpers::GetClass(&AIControllerClass, TEXT("/Game/Characters/Enemy/AI/BP_NoxController.BP_NoxController_C"));
	// AIControllerClass = ACEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 목표에 거리가 가까워지면서 속도가 줄어드는 현상 방지
	// GetCharacterMovement()->GetNavMovementProperties()->bUseFixedBrakingDistanceForPaths = false;
	GetCharacterMovement()->GetNavMovementProperties()->FixedPathBrakingDistance=0;
}

void ACNox_EBase::BeginPlay()
{
	Super::BeginPlay();

	if (bUseBehaviorTree)
		BehaviorComp->SetEnemyType(EnemyType);
}

void ACNox_EBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
