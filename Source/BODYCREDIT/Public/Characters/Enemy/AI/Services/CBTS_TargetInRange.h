// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTS_TargetInRange.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTS_TargetInRange : public UBTService
{
	GENERATED_BODY()

private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector MinRangeKey;
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector MaxRangeKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector IsInAttackRangeKey;
};
