#include "State/ZERO/CSplineMoveStrategy.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Patrol/CPatrolRoute.h"

CSplineMoveStrategy::CSplineMoveStrategy(ACPatrolRoute* InSpline): Spline(InSpline)
{
	const int32 NumPoints = Spline->GetNumberOfSplinePoints();
	for (int i = 0; i < NumPoints; ++i)
	{
		Distances.Add(Spline->GetDistanceAlongSplineAtSplinePoint(i));
	}
}

void CSplineMoveStrategy::Move(ACNox_EBase* Owner, float DeltaTime)
{
	if (!Spline || !Owner) return;

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
	if (DistToTarget < AcceptanceThreshold)	// 일정 거리에 도달했으면 다음 인덱스 변경
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