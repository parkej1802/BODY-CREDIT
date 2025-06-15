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

void ACSpawnBoundaryBox::AppendZeroCount()
{
	++SpawnCount.CurZeroSpawnCount;
}

void ACSpawnBoundaryBox::AppendMedicCount()
{
	++SpawnCount.CurMedicSpawnCount;
}

void ACSpawnBoundaryBox::AppendMemoryCount()
{
	++SpawnCount.CurMemorySpawnCount;
}

void ACSpawnBoundaryBox::ResetCount()
{
	SpawnCount.CurZeroSpawnCount = 0;
	SpawnCount.CurMedicSpawnCount = 0;
	SpawnCount.CurMemorySpawnCount = 0;
}

bool ACSpawnBoundaryBox::CanSpawnZero()
{
	return SpawnCount.CurZeroSpawnCount < SpawnCount.ZeroSpawnCount;
}

bool ACSpawnBoundaryBox::CanSpawnMedic()
{
	return SpawnCount.CurMedicSpawnCount < SpawnCount.MedicSpawnCount;
}

bool ACSpawnBoundaryBox::CanSpawnMemory()
{
	return SpawnCount.CurMemorySpawnCount < SpawnCount.MemorySpawnCount;
}
