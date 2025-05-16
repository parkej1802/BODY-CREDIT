// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTD_CheckCondition.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTD_CheckCondition : public UBTDecorator
{
	GENERATED_BODY()

private:
	UCBTD_CheckCondition();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(EditAnywhere, Category="Blackboard")
	TArray<FBlackboardKeySelector> KeyArray;
	UPROPERTY(EditAnywhere, Category="Blackboard")
	TArray<bool> ConditionArray;
};
