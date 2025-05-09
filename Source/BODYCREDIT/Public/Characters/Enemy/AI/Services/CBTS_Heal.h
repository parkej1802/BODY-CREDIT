// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTS_Heal.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTS_Heal : public UBTService
{
	GENERATED_BODY()

private:
	UCBTS_Heal();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
