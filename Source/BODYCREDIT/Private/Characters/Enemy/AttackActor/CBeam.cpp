#include "Characters/Enemy/AttackActor/CBeam.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"
#include "Global.h"
#include "Characters/CNox_Runner.h"

ACBeam::ACBeam()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<USceneComponent>(this, &rootScene, "rootScene");
	CHelpers::CreateComponent<UNiagaraComponent>(this, &LaserBeamVFX, "LaserBeamVFX", RootComponent);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &FireBallVFX, "FireBallVFX", RootComponent);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &HitVFX, "HitVFX", RootComponent);

	UNiagaraSystem* LaserBeamVFXSys = nullptr;
	// CHelpers::GetAsset<UNiagaraSystem>(&LaserBeamVFXSys,
	//                                    TEXT(
	// 	                                   "/Game/Assets/LaserBeam/Niagara/LaserBeam/NS_RibbonBeam1V3.NS_RibbonBeam1V3"));
	// if (LaserBeamVFXSys)
	// {
	// 	LaserBeamVFX->SetAsset(LaserBeamVFXSys);
	// 	LaserBeamVFX->SetAutoActivate(false);
	// }
	CHelpers::GetAsset<UNiagaraSystem>(&LaserBeamVFXSys,
	                                   TEXT(
		                                   "/Game/Assets/3D_Lasers/Effects/NS_Beam_3.NS_Beam_3"));
	if (LaserBeamVFXSys)
	{
		LaserBeamVFX->SetAsset(LaserBeamVFXSys);
		LaserBeamVFX->SetAutoActivate(false);
	}

	CHelpers::GetAsset<UNiagaraSystem>(&LaserBeamVFXSys,
	                                   TEXT("/Game/Assets/LaserBeam/Niagara/FireBall/NS_FireBall5.NS_FireBall5"));
	if (LaserBeamVFXSys)
	{
		FireBallVFX->SetAsset(LaserBeamVFXSys);
		FireBallVFX->SetAutoActivate(false);
	}

	// CHelpers::GetAsset<UNiagaraSystem>(&LaserBeamVFXSys,
	//                                    TEXT("/Game/Assets/LaserBeam/Niagara/HitPoint/NS_LiquidHit.NS_LiquidHit"));
	// if (LaserBeamVFXSys)
	// {
	// 	HitVFX->SetAsset(LaserBeamVFXSys);
	// 	HitVFX->SetAutoActivate(false);
	// }
	CHelpers::GetAsset<UNiagaraSystem>(&LaserBeamVFXSys,
	                                   TEXT("/Game/Assets/3D_Lasers/Effects/NS_LaserHit_3.NS_LaserHit_3"));
	if (LaserBeamVFXSys)
	{
		HitVFX->SetAsset(LaserBeamVFXSys);
		HitVFX->SetAutoActivate(false);
	}

	// TSubclassOf<UMaterialInstance> tmpDecal = nullptr;
	// CHelpers::GetClass<UMaterialInstance>(&tmpDecal,
	//                                       TEXT(
	// 	                                      "/Game/Assets/LaserBeam/Materials/Instance/MI_Fire_DecalGoldern.MI_Fire_DecalGoldern"));
	// if (tmpDecal)
	// 	FireDecal = tmpDecal.GetDefaultObject();
	// CHelpers::GetClass<UMaterialInstance>(&tmpDecal,
	//                                       TEXT(
	// 	                                      "/Game/Assets/LaserBeam/Materials/Decal/M_FireDecal.M_FireDecal"));
	// if (tmpDecal)
	// 	GlowDecal = tmpDecal.GetDefaultObject();
}

void ACBeam::BeginPlay()
{
	Super::BeginPlay();
	LaserBeamVFX->DeactivateImmediate();
	FireBallVFX->Deactivate();
	HitVFX->DeactivateImmediate();

	OwnerAI = Cast<ACNox_MemoryCollectorAI>(GetOwner());
}

void ACBeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TargetActor && AttackStart)
	{
		FVector start = rootScene->GetComponentLocation();
		// LaserBeamVFX->SetVariableVec3(FName("User.Start"), start);

		float newPitch = FMath::Lerp(
			CurPitch, (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation().Pitch,
			DeltaTime * 3);
		CurPitch = newPitch;
		
		FRotator NewRotation = FRotator(newPitch, 0, 0);
		FRotator aiRot = OwnerAI->GetActorRotation();
		// CLog::Print(FString::Printf(TEXT("AI Yaw : %.2f"), aiRot.Yaw));
		if (((180 > aiRot.Yaw && aiRot.Yaw > 90) || (-90 > aiRot.Yaw && aiRot.Yaw > -180))) NewRotation.Pitch *= -1;
		
		FVector Direction = NewRotation.RotateVector(OwnerAI->GetActorForwardVector() * AttackRange);
		FVector end = start + Direction;

		FHitResult HitResult;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		params.AddIgnoredActor(OwnerAI);
		if (GetWorld()->LineTraceSingleByChannel(HitResult, start, end, ECC_Camera, params))
		{
			// CLog::Print(
			// 	FString::Printf(TEXT("Hit OK, Distance : %.2f"), FVector::Distance(start, HitResult.Location)));
			// LaserBeamVFX->SetVariableVec3(FName("User.End"), HitResult.Location);
			LaserBeamVFX->SetVariableVec3(FName("User.LaserEnd"), HitResult.Location);
			if (!HitVFX->IsActive()) HitVFX->Activate(true);
			HitVFX->SetWorldLocation(HitResult.Location + 10);

			// UGameplayStatics::SpawnDecalAtLocation(this->GetWorld(), this->FireDecal,
			//                                        FVector(FMath::FRandRange(5.f, 20.f),
			//                                                FMath::FRandRange(5.f, 20.f),
			//                                                FMath::FRandRange(5.f, 20.f)),
			//                                        HitResult.Location, FRotator(-90.f, 0.f, 0.f), 2.f);
			// UGameplayStatics::SpawnDecalAtLocation(this->GetWorld(), this->GlowDecal,
			//                                        FVector(FMath::FRandRange(10.f, 40.f),
			//                                                FMath::FRandRange(10.f, 40.f),
			//                                                FMath::FRandRange(10.f, 40.f)),
			//                                        HitResult.Location, FRotator(-90.f, 0.f, 0.f), 4.f);
		}
		else
		{
			// CLog::Print(FString::Printf(TEXT("Hit NO, Distance : %.2f"), FVector::Distance(start, end)));
			// LaserBeamVFX->SetVariableVec3(FName("User.End"), end);
			LaserBeamVFX->SetVariableVec3(FName("User.LaserEnd"), end);
			HitVFX->DeactivateImmediate();
		}

		CurAttackDelay += DeltaTime;
		if (CurAttackDelay >= AttackDelay)
		{
			if (HitResult.GetActor() && HitResult.GetActor()->IsA(ACNox_Runner::StaticClass()))
			{
				// CLog::Log(FString::Printf(TEXT("Hit")));
			}
			CurAttackDelay = 0.f;
		}
	}
}

void ACBeam::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);
}

void ACBeam::SetBeamActive(bool bInActive, AActor* InTarget)
{
	SetActorHiddenInGame(!bInActive);
	TargetActor = InTarget;
	if (bInActive)
	{
		CurPitch = (InTarget->GetActorLocation() - GetActorLocation()).GetSafeNormal().Rotation().Pitch;		
		FireBallVFX->Activate(true);

		GetWorldTimerManager().SetTimer(ActiveTimer, [this]()
		{
			AttackStart = true;			
			LaserBeamVFX->Activate(true);			
			HitVFX->Activate(true);
		}, ActiveDelay, false);
	}
	else
	{
		AttackStart = false;
		// FireBallVFX->ResetSystem();
		FireBallVFX->Deactivate();
		
		// LaserBeamVFX->ResetSystem();
		// LaserBeamVFX->Deactivate();
		LaserBeamVFX->DeactivateImmediate();
		
		// HitVFX->ResetSystem();
		// HitVFX->Deactivate();
		HitVFX->DeactivateImmediate();
	}
}
