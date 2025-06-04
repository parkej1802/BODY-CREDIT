#include "Patrol/CPatrolRoute.h"

#include "Utilities/CHelpers.h"
#include "Components/SplineComponent.h"
#include "Utilities/CLog.h"

ACPatrolRoute::ACPatrolRoute()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &RootComp, "RootComp");
	CHelpers::CreateComponent<USplineComponent>(this, &SplineComp, "SplineComp", RootComponent);
}

void ACPatrolRoute::BeginPlay()
{
	Super::BeginPlay();
}

void ACPatrolRoute::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACPatrolRoute::IncrementPatrolRoute()
{
	PatrolRouteIndex += Direction;
	// CLog::Log(FString::Printf(TEXT("PatrolRouteIndex: %d"), PatrolRouteIndex));
	
	if (PatrolRouteIndex == SplineComp->GetNumberOfSplinePoints() - 1)
		Direction = -1;
	else if (PatrolRouteIndex == 0)
		Direction = 1;
}

FVector ACPatrolRoute::GetNextPointAsPosition()
{
	return SplineComp->GetLocationAtSplinePoint(PatrolRouteIndex, ESplineCoordinateSpace::World);
}

FVector ACPatrolRoute::GetLocationAtDistanceAlongSpline(const float InDistance, const ESplineCoordinateSpace::Type& CoordinateSpace) const
{
	return SplineComp->GetLocationAtDistanceAlongSpline(InDistance, CoordinateSpace);
}
