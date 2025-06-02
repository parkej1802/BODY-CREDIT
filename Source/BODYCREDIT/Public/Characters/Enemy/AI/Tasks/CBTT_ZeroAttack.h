// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_ZeroAttack.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_ZeroAttack : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UCBTT_ZeroAttack();
	
private:
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector MaxRangeKey;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
