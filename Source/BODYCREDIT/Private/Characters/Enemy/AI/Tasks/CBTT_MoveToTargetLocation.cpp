// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_MoveToTargetLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"
#include "Characters/Enemy/AI/CEnemyController.h"
#include "Navigation/PathFollowingComponent.h"

UCBTT_MoveToTargetLocation::UCBTT_MoveToTargetLocation()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_MoveToTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UCBTT_MoveToTargetLocation::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (ACEnemyController* controller = Cast<ACEnemyController>(OwnerComp.GetAIOwner()))
	{
		FVector TargetLocation = OwnerComp.GetBlackboardComponent()->
		                                   GetValueAsVector(TargetLocationKey.SelectedKeyName);
		EPathFollowingRequestResult::Type result = controller->
			MoveToLocation(TargetLocation, AcceptanceDistance, false);
		switch (result)
		{
		case EPathFollowingRequestResult::Failed:
			{
				// CLog::Log(FString::Printf(TEXT("Failed")));
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
			break;

		case EPathFollowingRequestResult::AlreadyAtGoal:
			{
				// CLog::Log(FString::Printf(TEXT("AlreadyAtGoal")));
				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}
			break;
		default:
			break;
		}
	}
	else
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}
