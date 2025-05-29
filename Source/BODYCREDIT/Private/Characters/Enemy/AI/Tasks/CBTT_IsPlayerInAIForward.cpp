// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_IsPlayerInAIForward.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_EBase.h"

EBTNodeResult::Type UCBTT_IsPlayerInAIForward::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;
	ACNox_EBase* SelfActor = Cast<ACNox_EBase>(OwnerComp.GetAIOwner()->GetPawn());
	ACNox* TargetActor = Cast<ACNox>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	float MaxAttackRange = Blackboard->GetValueAsFloat(MaxRangeKey.SelectedKeyName);

	if (SelfActor && TargetActor)
	{
		bool bInRange = SelfActor->IsPlayerInForwardRange(TargetActor, MaxAttackRange);
		return bInRange ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
