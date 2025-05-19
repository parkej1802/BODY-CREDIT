// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_SetMovementSpeed.generated.h"

enum class EEnemyMovementSpeed : uint8;
/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_SetMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	EEnemyMovementSpeed MovementSpeed;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
