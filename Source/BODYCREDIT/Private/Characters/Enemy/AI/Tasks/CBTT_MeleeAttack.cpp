// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_MeleeAttack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_EBase.h"

UCBTT_MeleeAttack::UCBTT_MeleeAttack()
{
	bCreateNodeInstance = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ACNox_EBase* MyEnemy = Cast<
		ACNox_EBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		float range = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(MeleeAttackRangeKey.SelectedKeyName);
		MyEnemy->HandleAttack(range);
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(MeleeAttackCoolTimeKey.SelectedKeyName, 0.f);
		return EBTNodeResult::InProgress;
	}
	else
		return EBTNodeResult::Failed;
}

void UCBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (ACNox_EBase* MyEnemy = Cast<
		ACNox_EBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		if (!MyEnemy->IsAttacking())
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}
