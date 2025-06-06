// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTS_TargetInDegree.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTS_TargetInDegree : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, Category=Blackboard)
	float Degree = 10.f;
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector RangeKey;
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector ConditionKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
