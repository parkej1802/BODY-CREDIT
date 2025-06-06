// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_MoveToMemory.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_MoveToMemory : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector MemoryTargetKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float AcceptanceDistance = 10;
	
	UCBTT_MoveToMemory();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
