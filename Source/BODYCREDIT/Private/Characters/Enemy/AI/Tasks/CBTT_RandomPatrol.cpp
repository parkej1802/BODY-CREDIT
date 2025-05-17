// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_RandomPatrol.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"
#include "Characters/Enemy/AI/CEnemyController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Global.h"

UCBTT_RandomPatrol::UCBTT_RandomPatrol()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_RandomPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ACNox_MemoryCollectorAI* MyEnemy = Cast<
		ACNox_MemoryCollectorAI>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		ACEnemyController* controller = Cast<ACEnemyController>(OwnerComp.GetAIOwner());
		if (!controller) return EBTNodeResult::Failed;
		
		if (UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(MyEnemy->GetWorld()))
		{
			FVector location = MyEnemy->GetActorLocation();
			FNavLocation point(location);
			while (true)
			{
				if (navSystem->GetRandomPointInNavigableRadius(location, RandomRadius, point))
					break;
			}

			if (bDebugMode)
				DrawDebugSphere(MyEnemy->GetWorld(), point.Location, 10, 10, FColor::Green, true, 5);

			MyEnemy->SetPatrolLocation(point.Location);

			return EBTNodeResult::InProgress;
		}
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}

void UCBTT_RandomPatrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (ACNox_MemoryCollectorAI* MyEnemy = Cast<
		ACNox_MemoryCollectorAI>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		if (ACEnemyController* controller = Cast<ACEnemyController>(OwnerComp.GetAIOwner()))
		{
			FVector location = MyEnemy->GetPatrolLocation();
			EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, AcceptanceDistance, false);
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
