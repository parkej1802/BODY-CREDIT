// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNox_CCTV.h"

ACNox_CCTV::ACNox_CCTV()
{
	EnemyType = EEnemyType::CCTV;
}

void ACNox_CCTV::BeginPlay()
{
	Super::BeginPlay();
	InitialRotation = GetActorRotation();
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

	// 회전이 끝점에 도달했는지 확인
	if ((bRotatingRight && NewYaw >= TargetYaw) || (!bRotatingRight && NewYaw <= TargetYaw))
	{
		NewYaw = TargetYaw;
		bIsPaused = true;
	}

	FRotator NewRotation = CurrentRotation;
	NewRotation.Yaw = NewYaw;
	SetActorRotation(NewRotation);
}
