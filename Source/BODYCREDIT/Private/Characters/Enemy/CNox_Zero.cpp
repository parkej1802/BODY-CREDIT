// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNox_Zero.h"

#include "EngineUtils.h"
#include "StaticMeshAttributes.h"
#include "Components/Enemy/CNox_BehaviorComponent.h"
#include "Engine/OverlapResult.h"
#include "Patrol/CPatrolRoute.h"
#include "Global.h"
#include "Characters/Enemy/CNoxEnemy_Animinstance.h"

ACNox_Zero::ACNox_Zero()
{
	EnemyType = EEnemyType::Zero;
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tmpMesh(TEXT(
		"/Game/Assets/Cyber_Zombie_Arm/Cyber_Zombie_Arm_UE4/SKM_Cyber_Zombie_Arm_UE4.SKM_Cyber_Zombie_Arm_UE4"));
	if (tmpMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(tmpMesh.Object);

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClass(
		TEXT("/Game/Characters/Enemy/Anim/ZeroAnim/ABP_ZeroAnim.ABP_ZeroAnim_C"));
	if (AnimInstanceClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimInstanceClass.Class);
}

void ACNox_Zero::BeginPlay()
{
	Super::BeginPlay();

	CHelpers::GetAssetDynamic(&(EnemyAnim->AttackMontage),
	                          TEXT("/Game/Assets/Cyber_Zombie_Arm/Anim/Attack/AM_Attack.AM_Attack"));

	GetNearPatrolRoute();
}

void ACNox_Zero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACNox_Zero::GetNearPatrolRoute()
{
	for (TActorIterator<ACPatrolRoute> It(GetWorld(), ACPatrolRoute::StaticClass()); It; ++It)
	{
		if (!NearPatrolRoute) NearPatrolRoute = *It;
		else
		{
			if (FVector::Dist((*It)->GetActorLocation(), GetActorLocation()) <
				FVector::Dist(NearPatrolRoute->GetActorLocation(), GetActorLocation()))
			{
				NearPatrolRoute = *It;
			}
		}
	}

	// BehaviorComp->SetPatrolRoute(NearPatrolRoute);
}

void ACNox_Zero::GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed,
                                     float& OutNewAccelSpeed)
{
	switch (InMovementSpeed)
	{
	case EEnemyMovementSpeed::Idle:
		OutNewSpeed = 0.f;
		OutNewAccelSpeed = 0.f;
		break;
	case EEnemyMovementSpeed::Walking:
		OutNewSpeed = 40.f;
		OutNewAccelSpeed = 128.f;
		break;
	// case EEnemyMovementSpeed::Jogging:
	// 	OutNewSpeed = 0.f;
	// 	OutNewAccelSpeed = 0.f;
	// 	break;
	case EEnemyMovementSpeed::Sprinting:
		OutNewSpeed = 310.f;
		OutNewAccelSpeed = 1024.f;
		break;
	}
}

void ACNox_Zero::SetDesiredRotation(const FRotator& InDesiredRotation)
{
	EnemyAnim->SetDesiredRotation(InDesiredRotation);
}
