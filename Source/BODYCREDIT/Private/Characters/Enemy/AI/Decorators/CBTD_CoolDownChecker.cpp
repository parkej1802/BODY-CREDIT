// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Decorators/CBTD_CoolDownChecker.h"

#include "BehaviorTree/BlackboardComponent.h"

UCBTD_CoolDownChecker::UCBTD_CoolDownChecker()
{
	bCreateNodeInstance = true;
}

bool UCBTD_CoolDownChecker::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	float CurrentCoolDown = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(CoolDownKey.SelectedKeyName);
	if (CurrentCoolDown >= CoolDownTime)
	{
		// OwnerComp.GetBlackboardComponent()->SetValueAsFloat(CoolDownKey.SelectedKeyName, 0.f);
		return true;
	}
	return false;
}
