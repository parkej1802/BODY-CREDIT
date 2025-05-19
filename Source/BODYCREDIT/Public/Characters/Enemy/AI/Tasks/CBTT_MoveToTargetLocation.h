// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_MoveToTargetLocation.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_MoveToTargetLocation : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector TargetLocationKey;
	UPROPERTY(EditAnywhere, Category=Blackboard)
	float AcceptanceDistance = 10;

	UCBTT_MoveToTargetLocation();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
