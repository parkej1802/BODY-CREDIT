// Fill out your copyright notice in the Description page of Project Settings.


#include "Patrol/CPatrolRoute.h"

#include "Utilities/CHelpers.h"
#include "Components/SplineComponent.h"
#include "Utilities/CLog.h"

// Sets default values
ACPatrolRoute::ACPatrolRoute()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &RootComp, "RootComp");
	CHelpers::CreateComponent<USplineComponent>(this, &SplineComp, "SplineComp", RootComponent);
}

// Called when the game starts or when spawned
void ACPatrolRoute::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
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
