#include "Characters/Enemy/AttackActor/CRangeProjectile.h"

#include "Characters/CNox_Runner.h"
#include "Components/BoxComponent.h"
#include "Global.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"

ACRangeProjectile::ACRangeProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<UBoxComponent>(this, &BoxComp, "BoxComp");
	BoxComp->SetCollisionProfileName(FName("EnemyWeapon"));
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ACRangeProjectile::OnOverlap);
}

void ACRangeProjectile::BeginPlay()
{
	Super::BeginPlay();
	OwnerAI = Cast<ACNox_MemoryCollectorAI>(GetOwner());
}

void ACRangeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target)
	{
		FVector ToTarget = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FVector CurrentDir = CurrentVelocity.GetSafeNormal();

		// 회전 제한 걸기
		// float MaxRadians = FMath::DegreesToRadians(MaxTurnRateDegPerSec) * DeltaTime;
		FVector NewDir = FMath::VInterpConstantTo(CurrentDir, ToTarget, DeltaTime, MaxTurnRateDegPerSec);
		NewDir = NewDir.GetSafeNormal();

		CurrentVelocity = NewDir * Speed;
		SetActorLocation(GetActorLocation() + CurrentVelocity * DeltaTime);
	}
}

void ACRangeProjectile::InitializeProjectile(FVector InStartLocation, ACNox* InTargetActor)
{
	Target = InTargetActor;
	StartLocation = InStartLocation;
	SetActorLocation(StartLocation);

	// 처음에는 정면으로 쏨
	FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	CurrentVelocity = Direction * Speed;
}

void ACRangeProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OwnerAI->ReturnToPool(this);
}

