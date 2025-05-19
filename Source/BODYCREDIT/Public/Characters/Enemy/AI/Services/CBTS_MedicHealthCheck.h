// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTS_MedicHealthCheck.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTS_MedicHealthCheck : public UBTService
{
	GENERATED_BODY()
	
private:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	const FName HealthKey = "Health";
};
