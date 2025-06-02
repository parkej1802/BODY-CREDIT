// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "Patrol/CPatrolRoute.h"
#include "CBTT_MoveAlongPatrolRoute.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_MoveAlongPatrolRoute : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTT_MoveAlongPatrolRoute();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY()
	UBehaviorTreeComponent* OwnerBT;
	UPROPERTY()
	class ACPatrolRoute* PatrolRoute;
	
	UFUNCTION()
	void OnPatrolResult(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	
	ACPatrolRoute* GetPatrolRoute(const UBehaviorTreeComponent& OwnerComp) const;
};
