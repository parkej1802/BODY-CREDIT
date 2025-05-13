// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTD_CoolDownChecker.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTD_CoolDownChecker : public UBTDecorator
{
	GENERATED_BODY()

private:
	UCBTD_CoolDownChecker();
	
private:
	UPROPERTY(EditAnywhere, Category = "Blackboard|CoolDownTime")
	float CoolDownTime=0.f;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector CoolDownKey;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
