#include "State/MEDIC/CRandomMoveStrategy.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Navigation/PathFollowingComponent.h"

void CRandomMoveStrategy::Move(ACNox_EBase* Owner, float DeltaTime)
{
	if (!Owner) return;
	if (Owner->bHearingMovement)
	{
		HearingMove(Owner);
	}
	else
		RandomMove(Owner);
}

void CRandomMoveStrategy::HearingMove(ACNox_EBase* Owner)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;
	
	// 이동속도 변경
	Owner->SetMovementSpeed(EEnemyMovementSpeed::Walking);
	EPathFollowingRequestResult::Type result = AICon->MoveToLocation(Owner->HearingLoc, AcceptanceThreshold, true);
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		Owner->bHearingMovement = false;
}

void CRandomMoveStrategy::RandomMove(ACNox_EBase* Owner)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;

	if (!bMoving)
	{
		RanLocation = GetRandomLocation(Owner); // 랜덤 위치 구하기
		if (Owner->bDebug)
			DrawDebugSphere(Owner->GetWorld(), RanLocation, 10, 10, FColor::Green, true, 1);
		bMoving = true;
		LastPosition = FVector::ZeroVector;
		StuckTime = 0.f;
	}
	else
	{
		// 이동 중 한 자리에 머무르는지 체크
		FVector CurrentPos = Owner->GetActorLocation();
		CurrentPos.Z = 0.f;
		
		if (LastPosition.IsZero())
		{
			LastPosition = CurrentPos;
			StuckTime = Owner->GetWorld()->GetTimeSeconds();
		}
		else
		{
			float DistMoved = FVector::Dist(CurrentPos, LastPosition);
			if (DistMoved < 10.f) // 10 유닛 이내로 움직임이 없으면
			{
				if (Owner->GetWorld()->GetTimeSeconds() - StuckTime >= 1.0f) // 1초 이상 머물러 있으면
				{
					bMoving = false;
					LastPosition = FVector::ZeroVector;
					StuckTime = 0.f;
					return;
				}
			}
			else
			{
				LastPosition = CurrentPos;
				StuckTime = Owner->GetWorld()->GetTimeSeconds();
			}
		}

		// 이동속도 변경
		Owner->SetMovementSpeed(EEnemyMovementSpeed::Walking);
		EPathFollowingRequestResult::Type result = AICon->MoveToLocation(RanLocation, AcceptanceThreshold, true);
		switch (result)
		{
		case EPathFollowingRequestResult::Failed:
		case EPathFollowingRequestResult::AlreadyAtGoal:
			bMoving = false;
			LastPosition = FVector::ZeroVector;
			StuckTime = 0.f;
			break;
		default:
			break;
		}
	}
}

FVector CRandomMoveStrategy::GetRandomLocation(const ACNox_EBase* Owner) const
{
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Owner->GetWorld());
	if (!navSystem) return FVector::ZeroVector;
	FVector location = Owner->GetActorLocation();
	FNavLocation point(location);
	while (true)
		if (navSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point)) break;
	return point.Location;
}
