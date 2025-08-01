#include "Characters/Enemy/AttackActor/CWavePulse.h"
#include "Global.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Components/TimelineComponent.h"

ACWavePulse::ACWavePulse()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &SphereComp, "SphereComp");
	CHelpers::GetStaticAsset<UStaticMeshComponent>(&SphereComp, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	SphereComp->SetCollisionProfileName(FName("EnemyWeapon"));
}

void ACWavePulse::BeginPlay()
{
	Super::BeginPlay();

	WaveTimeline = NewObject<UTimelineComponent>(this, FName("WaveTimeline"));
	WaveTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	WaveTimeline->SetNetAddressable();
	WaveTimeline->SetPropertySetObject(this);
	WaveTimeline->RegisterComponent();

	FOnTimelineFloat ProgressFunction;
	ProgressFunction.BindUFunction(this, FName("HandleWaveProgress"));
	WaveTimeline->AddInterpFloat(ScaleCurve, ProgressFunction);

	FOnTimelineEvent FinishedEvent;
	FinishedEvent.BindUFunction(this, FName("OnWaveEnd"));
	WaveTimeline->SetTimelineFinishedFunc(FinishedEvent);

	OwnerAI = Cast<ACNox_EBase>(GetOwner());
}

void ACWavePulse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACWavePulse::StartWave()
{
	SetActorHiddenInGame(false);
	WaveTimeline->PlayFromStart();
}

void ACWavePulse::HandleWaveProgress(float Value)
{
	float ScaleValue = FMath::Lerp(0.01f, MaxScale, Value);
	FVector NewScale(ScaleValue);
	this->SetActorScale3D(NewScale);

	// 회전: 대각선 방향 (Pitch + Yaw + Roll)
	FRotator DiagonalRotation = FMath::Lerp(FRotator::ZeroRotator, MaxRotation, Value);
	this->SetActorRotation(DiagonalRotation);
	
	// 충돌 감지
	TArray<AActor*> Overlaps;
	SphereComp->GetOverlappingActors(Overlaps, AActor::StaticClass());

	for (AActor* HitActor : Overlaps)
	{
		if (!HitActor || DamagedActors.Contains(HitActor)) continue;
		OwnerAI->SetApplyDamage(HitActor, 10.f);
		DamagedActors.Add(HitActor);
	}
}

void ACWavePulse::OnWaveEnd()
{
	SetActorHiddenInGame(true);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamagedActors.Empty();
}
