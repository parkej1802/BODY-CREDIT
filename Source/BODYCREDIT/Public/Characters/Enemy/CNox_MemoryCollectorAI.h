// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Data/CMemoryData.h"
#include "CNox_MemoryCollectorAI.generated.h"

/**
 * 기억 추적 AI
 */
UCLASS()
class BODYCREDIT_API ACNox_MemoryCollectorAI : public ACNox_EBase
{
	GENERATED_BODY()

public:
	ACNox_MemoryCollectorAI();

private:
	virtual void BeginPlay() override;
	virtual void SetPerceptionInfo() override;

private:
	UPROPERTY(EditAnywhere, Category="Memory")
	TArray<FMemoryFragment> MemoryQueue;

	UPROPERTY(EditAnywhere, Category="Memory")
	FMemoryFragment CurrentTargetMemory;

	float MemoryExpireTime = 15.0f; // TTL 기준

public:
	void RegisterMemory(const FMemoryFragment& InNewMemory);
	void EvaluateMemory();
	const FMemoryFragment GetMemoryTarget();
	FORCEINLINE bool IsMemoryEmpty() { return MemoryQueue.Num() > 0; }

public:
	void SetPatrolLocation(const FVector& InPatrolLocation);
	FVector GetPatrolLocation();

private:
	UPROPERTY()
	TArray<class ACStair*> AllStair;
	UPROPERTY()
	TArray<class ACVent*> AllVent;

public:
	TArray<class ACStair*> GetAllStair() const { return AllStair; }
	TArray<class ACVent*> GetAllVent() const { return AllVent; }
};
