#include "Characters/Enemy/CNox_Zero.h"

#include "EngineUtils.h"
#include "Components/Enemy/CNox_BehaviorComponent.h"
#include "Patrol/CPatrolRoute.h"
#include "Global.h"
#include "Characters/Enemy/CNoxEnemy_Animinstance.h"
#include "Components/BoxComponent.h"

ACNox_Zero::ACNox_Zero()
{
	EnemyType = EEnemyType::Zero;
	CHelpers::CreateComponent<UBoxComponent>(this, &AttackComp_l, "AttackComp_l", GetMesh(), "middle_01_l");
	CHelpers::CreateComponent<UBoxComponent>(this, &AttackComp_r, "AttackComp_r", GetMesh(), "middle_01_r");
	AttackComp_l->SetCollisionProfileName("EnemyWeapon");
	AttackComp_r->SetCollisionProfileName("EnemyWeapon");
	AttackComp_l->SetBoxExtent(FVector(25));
	AttackComp_r->SetBoxExtent(FVector(25));
		
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
	AttackCollision(false);
}

void ACNox_Zero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ACPatrolRoute* ACNox_Zero::GetNearPatrolRoute()
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
	return NearPatrolRoute;
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
	case EEnemyMovementSpeed::Sprinting:
		OutNewSpeed = 310.f;
		OutNewAccelSpeed = 1024.f;
		break;
	case EEnemyMovementSpeed::Combat:
		OutNewSpeed = 310.f;
		OutNewAccelSpeed = 1024.f;
		break;
	}
}

void ACNox_Zero::AttackCollision(bool bOn, bool IsRightHand)
{
	if (bOn)
	{
		if (IsRightHand)
		{
			AttackComp_r->SetCollisionEnabled(ECollisionEnabled::QueryOnly);			
			AttackComp_l->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else
		{
			AttackComp_l->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			AttackComp_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);			
		}
	}
	else
	{
		AttackComp_l->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		AttackComp_r->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
