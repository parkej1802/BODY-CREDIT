// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTS_EvaluateTransferPath.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTS_EvaluateTransferPath : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector MovementTypeKey;
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector MovementTargetLocationKey;
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector NextTargetAfterTransferKey;
	
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
