#include "State/ZERO/CSplineMoveStrategy.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "Patrol/CPatrolRoute.h"

CSplineMoveStrategy::CSplineMoveStrategy(ACPatrolRoute* InSpline)
{
	if (!InSpline) return;

	Spline = InSpline;
	const int32 NumPoints = Spline->GetNumberOfSplinePoints();
	for (int i = 0; i < NumPoints; ++i)
	{
		Distances.Add(Spline->GetDistanceAlongSplineAtSplinePoint(i));
	}
}

void CSplineMoveStrategy::Move(ACNox_EBase* Owner, float DeltaTime)
{
	if (!Owner) return;

	if (Spline)
	{
		SplineMove(Owner);
	}
	else
	{
		RandomMove(Owner);
	}
}

void CSplineMoveStrategy::ResetVal(ACNox_EBase* Owner)
{
	bMoving = false;
}

void CSplineMoveStrategy::SplineMove(ACNox_EBase* Owner)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;

	// 목적지 위치 가져오기 (World 기준 좌표)
	FVector TargetLocation = Spline->GetLocationAtDistanceAlongSpline(Distances[CurrentIndex],
	                                                                  ESplineCoordinateSpace::World);

	// Z축을 0으로 변경하여 거리 비교
	FVector OwnerLocation = Owner->GetActorLocation();
	OwnerLocation.Z = 0.f;
	TargetLocation.Z = 0.f;
	float DistToTarget = FVector::Dist(OwnerLocation, TargetLocation);
	if (bMoving && DistToTarget < AcceptanceThreshold) // 일정 거리에 도달했으면 다음 인덱스 변경
	{
		CurrentIndex = (++CurrentIndex) % Distances.Num();
		bMoving = false;
		return;
	}

	// 이동 중이 아니면
	if (!bMoving)
	{
		// 이동속도 변경
		Owner->SetMovementSpeed(EEnemyMovementSpeed::Walking);
		AICon->MoveToLocation(TargetLocation, AcceptanceRadius);
		bMoving = true;
	}
}

void CSplineMoveStrategy::RandomMove(ACNox_EBase* Owner)
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

FVector CSplineMoveStrategy::GetRandomLocation(const ACNox_EBase* Owner) const
{
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Owner->GetWorld());
	if (!navSystem) return FVector::ZeroVector;
	FVector location = Owner->GetActorLocation();
	FNavLocation point(location);
	while (true)
		if (navSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point)) break;
	return point.Location;
}
