// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_MoveToTarget_AddCondition.h"

#include "AIController.h"

UCBTT_MoveToTarget_AddCondition::UCBTT_MoveToTarget_AddCondition()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_MoveToTarget_AddCondition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	float distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(DirectionRadiusKey.SelectedKeyName);
	AcceptableRadius = distance;

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UCBTT_MoveToTarget_AddCondition::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	float CurrentCoolDown = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(ConditionKey.SelectedKeyName);
	if (CurrentCoolDown >= ConditionValue)
	{
		AAIController* AIController = OwnerComp.GetAIOwner();
		if (AIController)
		{
			// 이동을 중단합니다
			AIController->StopMovement();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
