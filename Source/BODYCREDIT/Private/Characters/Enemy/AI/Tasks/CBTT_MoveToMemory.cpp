// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_MoveToMemory.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"
#include "Characters/Enemy/AI/CEnemyController.h"
#include "Characters/Enemy/AI/Keys/CBBK_MemoryFragment.h"
#include "Navigation/PathFollowingComponent.h"

UCBTT_MoveToMemory::UCBTT_MoveToMemory()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_MoveToMemory::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::InProgress;
}

void UCBTT_MoveToMemory::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (ACNox_MemoryCollectorAI* MyEnemy = Cast<
		ACNox_MemoryCollectorAI>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		if (ACEnemyController* controller = Cast<ACEnemyController>(OwnerComp.GetAIOwner()))
		{
			auto& memory = MyEnemy->GetMemoryTarget();
			EPathFollowingRequestResult::Type result = controller->MoveToLocation(memory.Location, AcceptanceDistance, false);
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
	else
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}
