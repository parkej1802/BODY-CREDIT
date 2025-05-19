// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "CBTT_MoveToTarget_AddCondition.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_MoveToTarget_AddCondition : public UBTTask_MoveTo
{
	GENERATED_BODY()

private:
	UCBTT_MoveToTarget_AddCondition();
	
private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DirectionRadiusKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector ConditionKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float ConditionValue = 0.f;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
