// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTS_CoolDown.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTS_CoolDown : public UBTService
{
	GENERATED_BODY()

private:
	UCBTS_CoolDown();

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector CoolDownKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
