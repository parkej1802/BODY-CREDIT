// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNox_CCTV.h"

#include "Global.h"
#include "Characters/Enemy/AI/CEnemyController.h"
#include "Components/Enemy/CNoxEnemyHPComponent.h"
#include "Engine/OverlapResult.h"
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

	HPComp->SetStatus(50, 0);
}

void ACNox_CCTV::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HPComp->IsDead())
	{
		// 사망 시 움직이지 않는다.
	}
	else
	{
		RotateCCTV(DeltaTime);
	}
}

void ACNox_CCTV::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (auto* con = Cast<ACEnemyController>(NewController))
		con->OnDetectPlayer.BindUObject(this, &ACNox_CCTV::BroadCastDetectPlayer);
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

void ACNox_CCTV::BroadCastDetectPlayer(ACNox* DetectPlayer)
{
	FVector Origin = GetActorLocation();
	
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);  // 자기 자신은 제외
	
	bool bHit = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Origin,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),  // 폰만 검색
		FCollisionShape::MakeSphere(300.f),
		QueryParams
	);
	
	if (bHit)
	{
		for (auto& Result : OverlapResults)
		{
			ACNox_EBase* Enemy = Cast<ACNox_EBase>(Result.GetActor());
			if (!Enemy) continue;
	
			float ZDiff = FMath::Abs(Enemy->GetActorLocation().Z - Origin.Z);
			if (ZDiff <= 100.f)
			{
				// 플레이어 감지 정보 전달
				Enemy->SetTarget(DetectPlayer);
	
				// (선택) 디버그 표시
				// DrawDebugSphere(GetWorld(), Enemy->GetActorLocation(), 50.f, 8, FColor::Red, false, 1.0f);
			}
		}
	}
}
