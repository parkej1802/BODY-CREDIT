#include "Transportation/CStair.h"
#include "Global.h"

ACStair::ACStair()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root", RootComponent);
	CHelpers::CreateComponent<USceneComponent>(this, &PointA, "PointA");
	CHelpers::CreateComponent<USceneComponent>(this, &PointB, "PointB");
}

void ACStair::BeginPlay()
{
	Super::BeginPlay();
}

void ACStair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACStair::GetClosestEntryAndExit(const FVector& FromLocation, FVector& OutEntry, FVector& OutExit) const
{
	float DistA = FVector::Dist(FromLocation, PointA->GetComponentLocation());
	float DistB = FVector::Dist(FromLocation, PointB->GetComponentLocation());

	if (DistA < DistB)
	{
		OutEntry = PointA->GetComponentLocation();
		OutExit = PointB->GetComponentLocation();
	}
	else
	{
		OutEntry = PointB->GetComponentLocation();
		OutExit = PointA->GetComponentLocation();
	}
}
