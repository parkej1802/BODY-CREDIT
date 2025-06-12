#include "State/MEMORY/CRandomMoveStrategy_Memory.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Navigation/PathFollowingComponent.h"

void CRandomMoveStrategy_Memory::Move(ACNox_EBase* Owner, float DeltaTime)
{
	if (!Owner) return;
	
	
	RandomMove(Owner);
}

void CRandomMoveStrategy_Memory::RandomMove(ACNox_EBase* Owner)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;

	if (!bMoving)
	{
		RanLocation = GetRandomLocation(Owner); // 랜덤 위치 구하기
		DrawDebugSphere(Owner->GetWorld(), RanLocation, 10, 10, FColor::Green, true, 5);
		bMoving = true;
	}
	else
	{
		// 이동속도 변경
		Owner->SetMovementSpeed(EEnemyMovementSpeed::Walking);
		EPathFollowingRequestResult::Type result = AICon->MoveToLocation(RanLocation, AcceptanceThreshold, true);
		switch (result)
		{
		case EPathFollowingRequestResult::Failed:
		case EPathFollowingRequestResult::AlreadyAtGoal:
			bMoving = false;
			break;
		default:
			break;
		}
	}
}

FVector CRandomMoveStrategy_Memory::GetRandomLocation(const ACNox_EBase* Owner) const
{
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Owner->GetWorld());
	if (!navSystem) return FVector::ZeroVector;
	FVector location = Owner->GetActorLocation();
	FNavLocation point(location);
	while (true)
		if (navSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point)) break;
	return point.Location;
}
