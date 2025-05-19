// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_EquipShield.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_EquipShield : public UBTTaskNode
{
	GENERATED_BODY()

private:
	const FName EquipShieldKey = "bIsEquipShield";
	const FName ShieldKey = "ShieldAnimEnd";

	bool bIsEquipShield = false;
	
private:
	UCBTT_EquipShield();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category=HealTime)
	float HealTime = 2.0f;

	float CurHealTime = 0.0f;
};
