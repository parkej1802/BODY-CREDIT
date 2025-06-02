// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_ZeroAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_EBase.h"

UCBTT_ZeroAttack::UCBTT_ZeroAttack()
{
	bCreateNodeInstance = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_ZeroAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;
	ACNox_EBase* SelfActor = Cast<ACNox_EBase>(OwnerComp.GetAIOwner()->GetPawn());
	ACNox* TargetActor = Cast<ACNox>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	float MaxAttackRange = Blackboard->GetValueAsFloat(MaxRangeKey.SelectedKeyName);

	if (SelfActor && TargetActor)
	{
		bool bInRange = SelfActor->IsPlayerInForwardRange(TargetActor, MaxAttackRange);
		if (bInRange) {SelfActor->HandleAttack(0); return EBTNodeResult::InProgress;}
		else {return EBTNodeResult::Succeeded;}
	}
	else
		return EBTNodeResult::Succeeded;

	// if (ACNox_EBase* MyEnemy = Cast<
	// 	ACNox_EBase>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	// {
	// 	MyEnemy->HandleAttack(0);
	// 	return EBTNodeResult::InProgress;
	// }
	// else
	// 	return EBTNodeResult::Failed;
}

void UCBTT_ZeroAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
