// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Decorators/CBTD_HavingMemoryPiece.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"

bool UCBTD_HavingMemoryPiece::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (ACNox_MemoryCollectorAI* MyEnemy = Cast<
		ACNox_MemoryCollectorAI>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		return MyEnemy->IsMemoryEmpty();
	}
	
	return false;
}
