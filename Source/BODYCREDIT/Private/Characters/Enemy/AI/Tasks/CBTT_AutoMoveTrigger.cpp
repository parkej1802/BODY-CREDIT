// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_AutoMoveTrigger.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_EBase.h"


UCBTT_AutoMoveTrigger::UCBTT_AutoMoveTrigger()
{
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UCBTT_AutoMoveTrigger::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ACNox_EBase* MyEnemy = Cast<
		ACNox_EBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		ACNox* target = Cast<ACNox>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
		float distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(DistanceKey.SelectedKeyName);
		MyEnemy->SetAutoMove(bTrigger, target, distance);
		
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
