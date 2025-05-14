// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNox_EBase.h"

#include "global.h"
#include "Characters/Enemy/CNoxEnemy_Animinstance.h"
#include "Components/Enemy/CNox_BehaviorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/behaviorTree.h"
#include "Characters/Enemy/AI/CEnemyController.h"
#include "Components/Enemy/CNoxEnemyHPComponent.h"
#include "Navigation/PathFollowingComponent.h"

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
	GetCharacterMovement()->GetNavMovementProperties()->FixedPathBrakingDistance = 0;

	CHelpers::GetAsset(&BehaviorTree,
	                   TEXT("/Game/Characters/Enemy/AI/BT_Nox.BT_Nox"));
	CHelpers::CreateActorComponent<UCNoxEnemyHPComponent>(this, &HPComp, "HPComp");
}

void ACNox_EBase::BeginPlay()
{
	Super::BeginPlay();

	if (bUseBehaviorTree)
	{
		BehaviorComp->SetEnemyType(EnemyType);
	}


	if (auto Anim = GetMesh()->GetAnimInstance())
	{
		EnemyAnim = Cast<UCNoxEnemy_Animinstance>(Anim);
		EnemyAnim->SetEnemy(this);
		if (bUseBehaviorTree)
			EnemyAnim->SetBT(BehaviorComp);
	}

	if (HPComp)
		HPComp->SetEnemy(this);
}

void ACNox_EBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bAutoMove && Target)
	{
		float dist = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
		if (dist > MoveDistance)
		{
			// CLog::Log(FString::Printf(TEXT("MoveDistance: %f, dist: %f"), MoveDistance, dist));
			FAIMoveRequest request;
			request.SetGoalActor(Target);
			request.SetAcceptanceRadius(MoveDistance);
			FPathFollowingRequestResult result = EnemyController->MoveTo(request);
			// FVector DirectionVector = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			// AddMovementInput(DirectionVector);
		}
	}
}

void ACNox_EBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	EnemyController = Cast<ACEnemyController>(NewController);
}

void ACNox_EBase::SetTarget(ACNox* InTarget)
{
	BehaviorComp->SetTarget(InTarget);
}

void ACNox_EBase::HealHP()
{
	HPComp->HealHP(HealAmount);
}

void ACNox_EBase::SetGrenadeEnded(bool InbEndedAnim)
{
	BehaviorComp->SetGrenadeEnded(InbEndedAnim);
}
