// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTD_CheckOtherFloor.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTD_CheckOtherFloor : public UBTDecorator
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector MemoryTargetKey;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float Distance = 100;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
