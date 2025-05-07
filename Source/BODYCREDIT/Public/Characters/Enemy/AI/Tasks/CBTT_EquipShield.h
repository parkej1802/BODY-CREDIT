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
	const FName GrenadeEndKey = "GrenadeEnd";
	const FName CanUseShieldKey = "bCanUseShield";
	const FName HitDamageKey = "bHitDamage";
	const FName EquipShieldKey = "bIsEquipShield";
	const FName BrokenShieldKey = "bIsBrokenShield";
	const FName ShieldKey = "ShieldAnimEnd";

	bool bGrenadeEnd = false;
	bool bCanUseShield = false;
	bool bHitDamage = false;
	bool bIsEquipShield = false;
	bool bIsBrokenShield = false;
	
private:
	UCBTT_EquipShield();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
