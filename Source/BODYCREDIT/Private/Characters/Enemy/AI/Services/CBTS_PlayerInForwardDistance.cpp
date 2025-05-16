// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Services/CBTS_PlayerInForwardDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_EBase.h"

UCBTS_PlayerInForwardDistance::UCBTS_PlayerInForwardDistance()
{
	bCreateNodeInstance = true;
}

void UCBTS_PlayerInForwardDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return;
	
	ACNox_EBase* SelfActor = Cast<ACNox_EBase>(OwnerComp.GetAIOwner()->GetPawn());
	ACNox* TargetActor = Cast<ACNox>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	float MaxAttackRange = Blackboard->GetValueAsFloat(MaxRangeKey.SelectedKeyName);

	if (SelfActor && TargetActor)
	{
		bool bInRange = SelfActor->IsPlayerInForwardRange(TargetActor, MaxAttackRange);
		Blackboard->SetValueAsBool(IsInAttackRangeKey.SelectedKeyName, bInRange);
	}
	else
	{
		Blackboard->SetValueAsBool(IsInAttackRangeKey.SelectedKeyName, false);
	}
}
