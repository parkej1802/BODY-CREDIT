#include "Trigger/CSpawnBoundaryBox.h"
#include "Global.h"
#include "Components/BoxComponent.h"

ACSpawnBoundaryBox::ACSpawnBoundaryBox()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent(this, &BoxComp, "BoxComp");
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACSpawnBoundaryBox::BeginPlay()
{
	Super::BeginPlay();
}

void ACSpawnBoundaryBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ACSpawnBoundaryBox::GetLoc() const
{
	return BoxComp->GetComponentLocation();
}

FVector ACSpawnBoundaryBox::GetScale() const
{
	return BoxComp->GetScaledBoxExtent();
}
