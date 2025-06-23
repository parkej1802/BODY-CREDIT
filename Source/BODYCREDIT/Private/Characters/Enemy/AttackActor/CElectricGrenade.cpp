#include "Characters/Enemy/AttackActor/CElectricGrenade.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "Characters/CNox_Runner.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACElectricGrenade::ACElectricGrenade()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USceneComponent>(this, &RootComp, "RootComp");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &MeshComp, "MeshComp", RootComp);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &ProjectileComp, "ProjectileComp");
	ProjectileComp->bRotationFollowsVelocity = true;
	ProjectileComp->bAutoActivate = false;
	CHelpers::CreateComponent<UNiagaraComponent>(this, &FlashFX, "FlashFX", RootComp);
	FlashFX->bAutoActivate = false;

	MeshComp->SetCollisionProfileName(FName("EnemyWeapon"));
}

void ACElectricGrenade::BeginPlay()
{
	Super::BeginPlay();
	UseFX(false);
	Init(false);

	OwnerAI = Cast<ACNox_EBase>(GetOwner());
}

void ACElectricGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector CurrentLocation = GetActorLocation();
	FVector Delta = CurrentLocation - TargetLocation;

	// Z축 무시한 평면 거리
	Delta.Z = 0;

	if (!bExploded && Delta.Size() <= TriggerRadius)
	{
		Explode(); // 폭발 처리 함수
		bExploded = true;
	}
}

void ACElectricGrenade::Explode()
{
	FVector Origin = GetActorLocation();
	float Radius = 500.f; // 폭발 반경
	
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 수류탄 자신 제외
	Params.AddIgnoredActor(GetOwner());

	if (OwnerAI->bDebug)
		DrawDebugSphere(GetWorld(), Origin, Radius, 10, FColor::Red, true, 1.0f);
	GetWorld()->OverlapMultiByChannel(
		Overlaps,
		Origin,
		FQuat::Identity,
		ECC_GameTraceChannel9,
		FCollisionShape::MakeSphere(Radius),
		Params
	);

	for (const FOverlapResult& Result : Overlaps)
	{
		AActor* Target = Result.GetActor();
		if (!Target) continue;

		if (ACNox_Runner* Runner = Cast<ACNox_Runner>(Target))
		{
			FVector PlayerLocation = Target->GetActorLocation();
			FHitResult Hit;
			FCollisionQueryParams TraceParams;
			TraceParams.AddIgnoredActor(this);
			TraceParams.AddIgnoredActor(Target);

			bool bBlocked = GetWorld()->LineTraceSingleByChannel(
				Hit,
				Origin,
				PlayerLocation,
				ECC_Visibility,
				TraceParams
			);

			if (!bBlocked || (bBlocked && Hit.GetActor() == Target))
				Runner->ReactFlashBang(GetActorLocation());
		}
	}

	// 이펙트 처리
	UseFX(true);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		UseFX(false);
		Init(false);
	}, 1.f, false);
}

void ACElectricGrenade::Init(bool bInit)
{
	if (bInit)
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		this->SetActorHiddenInGame(false);
		ProjectileComp->Activate(true);
		SetActorTickEnabled(true);
		bExploded = false;
	}
	else
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		this->SetActorHiddenInGame(true);
		ProjectileComp->Activate(false);
		SetActorTickEnabled(false);
	}
}

void ACElectricGrenade::UseFX(bool bUse)
{
	bUse ? FlashFX->Activate(true) : FlashFX->DeactivateImmediate();
}

void ACElectricGrenade::InitializeGrenade(const FVector& InStartLocation, const FVector& InTargetLocation,
                                          const FVector& InVelocity, const float InMaxSpeed)
{
	SetActorLocation(InStartLocation);
	ProjectileComp->Velocity = InVelocity;
	ProjectileComp->InitialSpeed = InVelocity.Size();
	ProjectileComp->MaxSpeed = InMaxSpeed;
	ProjectileComp->bShouldBounce = false;
	TargetLocation = InTargetLocation;
	Init(true);
}
