// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNox_CCTV.h"

#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

ACNox_CCTV::ACNox_CCTV()
{
	EnemyType = EEnemyType::Cctv;

	GetCapsuleComponent()->SetCapsuleRadius(32.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(32.f);
	GetCharacterMovement()->GravityScale = 0.f;

	CHelpers::CreateComponent(this, &CCTVMesh, "CCTVMesh", RootComponent);
	CCTVMesh->SetRelativeLocation(FVector(0, 0, -14));
	CCTVMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CHelpers::GetStaticAsset<UStaticMeshComponent>(
		&CCTVMesh, TEXT("/Game/Assets/cctv_prop/source/prop_cctv_cam_01a_001.prop_cctv_cam_01a_001"));

	bUseBehaviorTree = false;
}

void ACNox_CCTV::BeginPlay()
{
	Super::BeginPlay();
	InitialRotation = GetActorRotation();
	SumRotYaw = InitialRotation.Yaw;
}

void ACNox_CCTV::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateCCTV(DeltaTime);
}

void ACNox_CCTV::RotateCCTV(float DeltaTime)
{
	// 회전을 멈추고, 일정 시간이 지나면 다시 회전하도록 한다
	if (bIsPaused)
	{
		PauseTimer += DeltaTime;
		if (PauseTimer >= PauseTimeAtEnds)
		{
			PauseTimer = 0.f;
			bIsPaused = false;
			bRotatingRight = !bRotatingRight;
		}
		return;
	}

	FRotator CurrentRotation = GetActorRotation();
	float TargetYaw = bRotatingRight
		                  ? InitialRotation.Yaw + MaxYaw
		                  : InitialRotation.Yaw + MinYaw;

	float Direction = bRotatingRight ? 1.f : -1.f;
	float YawDelta = RotationSpeed * DeltaTime * Direction;
	float NewYaw = CurrentRotation.Yaw + YawDelta;
	SumRotYaw += YawDelta;

	// 회전이 끝점에 도달했는지 확인
	// CLog::Log(FString::Printf(
	// 	TEXT("bRotatingRight: %d, SumRotYaw: %.1f, TargetYaw: %.1f"), bRotatingRight, SumRotYaw, TargetYaw));
	if ((bRotatingRight && SumRotYaw >= TargetYaw) || (!bRotatingRight && SumRotYaw <= TargetYaw))
	{
		NewYaw = TargetYaw;
		bIsPaused = true;
	}

	FRotator NewRotation = CurrentRotation;
	NewRotation.Yaw = NewYaw;
	SetActorRotation(NewRotation);
}
