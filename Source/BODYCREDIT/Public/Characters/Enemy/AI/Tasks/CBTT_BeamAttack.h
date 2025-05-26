// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_BeamAttack.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_BeamAttack : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UCBTT_BeamAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
