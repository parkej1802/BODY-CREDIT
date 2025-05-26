// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_EvaluateMemoryTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"

EBTNodeResult::Type UCBTT_EvaluateMemoryTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ACNox_MemoryCollectorAI* MyEnemy = Cast<
		ACNox_MemoryCollectorAI>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		MyEnemy->EvaluateMemory();
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
