// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_SetMovementSpeed.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_EBase.h"

EBTNodeResult::Type UCBTT_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ACNox_EBase* MyEnemy = Cast<
		ACNox_EBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		MyEnemy->SetMovementSpeed(MovementSpeed);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
