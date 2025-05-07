// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_Grenade.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_Grenade : public UBTTaskNode
{
	GENERATED_BODY()

	const FName GrenadeKey = "GrenadeEnd";
	const FName GrenadeCanUseKey = "bCanUseElectricGrenade";
private:
	UCBTT_Grenade();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
