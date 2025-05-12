// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNox_MemoryCollectorAI.h"

#include "Characters/Enemy/CNoxEnemy_Animinstance.h"
#include "Global.h"

ACNox_MemoryCollectorAI::ACNox_MemoryCollectorAI()
{
	EnemyType = EEnemyType::MemoryCollector;
}

void ACNox_MemoryCollectorAI::BeginPlay()
{
	Super::BeginPlay();
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
