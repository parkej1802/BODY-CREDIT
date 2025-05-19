#include "Transportation/CVent.h"
#include "Global.h"

ACVent::ACVent()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root", RootComponent);
}

void ACVent::BeginPlay()
{
	Super::BeginPlay();
}

void ACVent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ACVent::GetLocation() const
{
	return GetActorLocation();
}

