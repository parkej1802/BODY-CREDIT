#include "Characters/Enemy/AttackActor/CBeam.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"
#include "Characters/CNox_Runner.h"

ACBeam::ACBeam()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 컴포넌트 생성
	CreateComponents();
	
	// VFX 에셋 설정
	SetupVFXAssets();
}

void ACBeam::CreateComponents()
{
	CHelpers::CreateComponent<USceneComponent>(this, &rootScene, "rootScene");
	CHelpers::CreateComponent<UNiagaraComponent>(this, &LaserBeamVFX, "LaserBeamVFX", RootComponent);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &FireBallVFX, "FireBallVFX", RootComponent);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &HitVFX, "HitVFX", RootComponent);
}

void ACBeam::SetupVFXAssets()
{
	// 레이저 빔 VFX 설정
	UNiagaraSystem* NiagaraSys = nullptr;
	CHelpers::GetAsset<UNiagaraSystem>(&NiagaraSys, TEXT("/Game/Assets/3D_Lasers/Effects/NS_Beam_3.NS_Beam_3"));
	if (NiagaraSys)
	{
		LaserBeamVFX->SetAsset(NiagaraSys);
		LaserBeamVFX->SetAutoActivate(false);
	}

	// 파이어볼 VFX 설정
	CHelpers::GetAsset<UNiagaraSystem>(&NiagaraSys, TEXT("/Game/Assets/LaserBeam/Niagara/FireBall/NS_FireBall5.NS_FireBall5"));
	if (NiagaraSys)
	{
		FireBallVFX->SetAsset(NiagaraSys);
		FireBallVFX->SetAutoActivate(false);
	}

	// 히트 VFX 설정
	CHelpers::GetAsset<UNiagaraSystem>(&NiagaraSys, TEXT("/Game/Assets/3D_Lasers/Effects/NS_LaserHit_3.NS_LaserHit_3"));
	if (NiagaraSys)
	{
		HitVFX->SetAsset(NiagaraSys);
		HitVFX->SetAutoActivate(false);
	}
}

void ACBeam::BeginPlay()
{
	Super::BeginPlay();
	
	// VFX 초기화
	DeactivateAllVFX();
	
	// AI 컨트롤러 설정
	OwnerAI = Cast<ACNox_MemoryCollectorAI>(GetOwner());
}

void ACBeam::DeactivateAllVFX()
{
	LaserBeamVFX->DeactivateImmediate();
	FireBallVFX->Deactivate();
	HitVFX->DeactivateImmediate();
}

void ACBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateDamageTimer(DeltaTime);
	
	if (TargetActor && AttackStart)
	{
		UpdateBeamPosition(DeltaTime);
		UpdateAttackDelay(DeltaTime);
	}
}

void ACBeam::UpdateDamageTimer(float DeltaTime)
{
	if (bApplyDamage)
	{
		if (DamageTimer <= DamageInterval)
		{
			DamageTimer += DeltaTime;
		}
		else
		{
			bApplyDamage = false;
		}
	}
}

void ACBeam::UpdateBeamPosition(float DeltaTime)
{
	FVector start = rootScene->GetComponentLocation();
	FVector end = CalculateBeamEnd(DeltaTime);
	
	// 레이캐스트 수행
	FHitResult HitResult;
	if (PerformLineTrace(start, end, HitResult))
	{
		HandleBeamHit(HitResult);
	}
	else
	{
		HandleBeamMiss(end);
	}
}

FVector ACBeam::CalculateBeamEnd(float DeltaTime)
{
	// 피치 각도 계산
	float newPitch = FMath::Lerp(
		CurPitch, 
		(TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation().Pitch,
		DeltaTime * 3
	);
	CurPitch = newPitch;

	// 회전값 계산
	FRotator NewRotation = FRotator(newPitch, 0, 0);
	if (!OwnerAI) return FVector::ZeroVector;
	FRotator aiRot = OwnerAI->GetActorRotation();
	if (((180 > aiRot.Yaw && aiRot.Yaw > 90) || (-90 > aiRot.Yaw && aiRot.Yaw > -180))) 
		NewRotation.Pitch *= -1;

	// 빔 방향 계산
	FVector Direction = NewRotation.RotateVector(OwnerAI->GetActorForwardVector() * AttackRange);
	return rootScene->GetComponentLocation() + Direction;
}

bool ACBeam::PerformLineTrace(const FVector& Start, const FVector& End, FHitResult& OutHitResult)
{
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(OwnerAI);
	
	return GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECC_Visibility, params);
}

void ACBeam::HandleBeamHit(const FHitResult& HitResult)
{
	LaserBeamVFX->SetVariableVec3(FName("User.LaserEnd"), HitResult.Location);
	
	if (!HitVFX->IsActive()) 
		HitVFX->Activate(true);
	HitVFX->SetWorldLocation(HitResult.Location + 10);

	if (!bApplyDamage && HitResult.GetActor()->IsA(ACNox_Runner::StaticClass()))
		OwnerAI->SetApplyDamage(HitResult.GetActor(), 10.f);
}

void ACBeam::HandleBeamMiss(const FVector& EndLocation)
{
	LaserBeamVFX->SetVariableVec3(FName("User.LaserEnd"), EndLocation);
	HitVFX->DeactivateImmediate();
}

void ACBeam::UpdateAttackDelay(float DeltaTime)
{
	CurAttackDelay += DeltaTime;
	if (CurAttackDelay >= AttackDelay)
	{
		CurAttackDelay = 0.f;
	}
}

void ACBeam::SetBeamActive(bool bInActive, AActor* InTarget)
{
	SetActorHiddenInGame(!bInActive);
	TargetActor = InTarget;
	
	if (bInActive)
	{
		InitializeBeam(InTarget);
	}
	else
	{
		DeactivateBeam();
	}
}

void ACBeam::InitializeBeam(AActor* InTarget)
{
	DamageTimer = 0.0f;
	bApplyDamage = false;
	
	CurPitch = (InTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation().Pitch;
	FireBallVFX->Activate(true);

	GetWorldTimerManager().SetTimer(ActiveTimer, [this]()
	{
		AttackStart = true;
		LaserBeamVFX->Activate(true);
		HitVFX->Activate(true);
	}, ActiveDelay, false);
}

void ACBeam::DeactivateBeam()
{
	AttackStart = false;
	FireBallVFX->Deactivate();
	LaserBeamVFX->DeactivateImmediate();
	HitVFX->DeactivateImmediate();
}
