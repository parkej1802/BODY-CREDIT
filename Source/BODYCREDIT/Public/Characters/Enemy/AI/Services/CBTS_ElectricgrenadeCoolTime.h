// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTS_ElectricgrenadeCoolTime.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTS_ElectricgrenadeCoolTime : public UBTService
{
	GENERATED_BODY()

	const FName ElectricGrenadeCoolTimeKey = "bCanUseElectricGrenade";
private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
