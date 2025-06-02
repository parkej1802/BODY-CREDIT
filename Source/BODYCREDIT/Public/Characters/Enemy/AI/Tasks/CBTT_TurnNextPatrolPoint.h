// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_TurnNextPatrolPoint.generated.h"

struct FBTTask_RotateMemory
{
	FRotator TargetRotation; // 목표 회전 값 저장
	bool bRotationSet = false; // 회전값 세팅 여부
};

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_TurnNextPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UCBTT_TurnNextPatrolPoint();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float RotationSpeed = 180.f;
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float AcceptableAngle = 5.f;
};
