// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_AutoMoveTrigger.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_AutoMoveTrigger : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UCBTT_AutoMoveTrigger();

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DistanceKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	bool bTrigger = false;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
