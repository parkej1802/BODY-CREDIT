// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_MeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UCBTT_MeleeAttack();
	
private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MeleeAttackRangeKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MeleeAttackCoolTimeKey;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
