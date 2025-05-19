// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_StopMovement.h"

#include "AIController.h"

EBTNodeResult::Type UCBTT_StopMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	// AIController 가져오기
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		// 이동을 멈추려면 StopMovement 사용
		AIController->StopMovement();

		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
