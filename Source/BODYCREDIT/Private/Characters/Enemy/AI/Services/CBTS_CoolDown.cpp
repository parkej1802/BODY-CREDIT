// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Services/CBTS_CoolDown.h"

#include "BehaviorTree/BlackboardComponent.h"

UCBTS_CoolDown::UCBTS_CoolDown()
{
	bCreateNodeInstance = true;
}

void UCBTS_CoolDown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	float CurrentCoolDown = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(CoolDownKey.SelectedKeyName);
	CurrentCoolDown+=DeltaSeconds;
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(CoolDownKey.SelectedKeyName, CurrentCoolDown);
}
