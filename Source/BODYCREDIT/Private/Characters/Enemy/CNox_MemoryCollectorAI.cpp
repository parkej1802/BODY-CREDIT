// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNox_MemoryCollectorAI.h"

#include "Characters/Enemy/CNoxEnemy_Animinstance.h"
#include "Global.h"
#include "Characters/Enemy/AI/CEnemyController.h"
#include "Navigation/PathFollowingComponent.h"

ACNox_MemoryCollectorAI::ACNox_MemoryCollectorAI()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tmpMesh(TEXT(
			"/Game/Assets/Sci_Fi_Characters_Pack/Mesh/Sci_Fi_Character_04/SK_Sci_Fi_Character_04_Full.SK_Sci_Fi_Character_04_Full"));
	if (tmpMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(tmpMesh.Object);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -85), FRotator(0, -90, 0));
	GetMesh()->SetRelativeScale3D(FVector(1.1));

	GetCapsuleComponent()->SetCapsuleHalfHeight(95.f);
	GetCapsuleComponent()->SetCapsuleRadius(34.f);

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClass(
		TEXT("/Game/Characters/Enemy/Anim/MemoryAnim/ABP_MemoryAnim.ABP_MemoryAnim_C"));
	if (AnimInstanceClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimInstanceClass.Class);

	EnemyType = EEnemyType::MemoryCollector;
	SetPerceptionInfo();
}

void ACNox_MemoryCollectorAI::BeginPlay()
{
	Super::BeginPlay();
}

void ACNox_MemoryCollectorAI::SetPerceptionInfo()
{
	Super::SetPerceptionInfo();

	RetentionTime = 0.f;
}

void ACNox_MemoryCollectorAI::RegisterMemory(const FMemoryFragment& InNewMemory)
{
	if (!HasAuthority()) return;

	// TTL 체크: 오래된 기억 제거
	const float Now = GetWorld()->GetTimeSeconds();
	MemoryQueue.RemoveAll([Now, this](const FMemoryFragment& Mem)
	{
		return (Now - Mem.TimeStamp) > MemoryExpireTime;
	});

	// TTL 삭제 후 현재 기억이 포함됐는지 검사
	if (!MemoryQueue.Contains(CurrentTargetMemory))
	{
		CurrentTargetMemory = FMemoryFragment(); // 초기화
		// // 또는 다음 우선순위 기억으로 갱신
		// if (MemoryQueue.Num() > 0)
		// {
		// 	CurrentTargetMemory = MemoryQueue[0];
		// }
	}

	// 중복 방지: 동일 위치/플레이어 기억은 덮어쓰기 또는 무시
	for (FMemoryFragment& Mem : MemoryQueue)
	{
		if (Mem.TriggerType == InNewMemory.TriggerType &&
			Mem.ZoneID == InNewMemory.ZoneID)
		{
			if (InNewMemory.TimeStamp > Mem.TimeStamp)
			{
				Mem = InNewMemory; // 더 최신이면 갱신
			}
			return; // 중복 방지
		}
	}

	// 새로운 기억 추가
	MemoryQueue.Add(InNewMemory);

	// 우선순위 재계산
	FVector AILocation = GetActorLocation();
	for (FMemoryFragment& Mem : MemoryQueue)
	{
		Mem.CalculatePriority(AILocation);
	}

	MemoryQueue.Sort(); // operator< 로 우선순위 내림차순 정렬

	// 가장 우선순위가 높은 기억으로 현재 목표 갱신
	if (MemoryQueue.Num() > 0 && MemoryQueue[0].CalculatedPriority > CurrentTargetMemory.CalculatedPriority)
	{
		CurrentTargetMemory = MemoryQueue[0];
		// Blackboard에 등록된 위치 갱신 등 추가 가능
	}
}

void ACNox_MemoryCollectorAI::SetPatrolLocation(const FVector& InPatrolLocation)
{
	BehaviorComp->SetPatrolRandomLocation(InPatrolLocation);

	// FAIMoveRequest MoveRequest;
	// MoveRequest.SetGoalLocation(InPatrolLocation);
	// MoveRequest.SetAcceptanceRadius(10.0f); // ← 이 값을 넉넉히 주세요
	//
	// FNavPathSharedPtr NavPath;
	// auto Result = EnemyController->MoveTo(MoveRequest, &NavPath);
	// DrawDebugSphere(GetWorld(), InPatrolLocation, 30.f, 12, FColor::Green, false, 5.f);
	// if (Result != EPathFollowingRequestResult::Type::RequestSuccessful)
	// {
	// 	CLog::Log(FString::Printf(TEXT("Request Failed")));
	// }
}

FVector ACNox_MemoryCollectorAI::GetPatrolLocation()
{
	return BehaviorComp->GetPatrolRandomLocation();
}
