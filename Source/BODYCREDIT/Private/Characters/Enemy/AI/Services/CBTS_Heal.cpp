// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Services/CBTS_Heal.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"

UCBTS_Heal::UCBTS_Heal()
{
	bCreateNodeInstance = true;
}

void UCBTS_Heal::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if(auto* SelfActor = Cast<ACNox_EBase>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		SelfActor->HealHP(); 
	}
}
