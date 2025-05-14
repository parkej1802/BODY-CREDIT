// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Decorators/CBT_CheckCondition.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Utilities/CLog.h"

UCBT_CheckCondition::UCBT_CheckCondition()
{
	FlowAbortMode = EBTFlowAbortMode::Self;
	bCreateNodeInstance = true;
}

bool UCBT_CheckCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (KeyArray.Num() != ConditionArray.Num()) return false;
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return false;

	int32 ArrayNum = KeyArray.Num();
	for (int i = 0; i < ArrayNum; ++i)
	{
		bool bCondition = Blackboard->GetValueAsBool(KeyArray[i].SelectedKeyName);
		// CLog::Log(FString::Printf(TEXT("%s: %d"), *KeyArray[i].SelectedKeyName.ToString(), bCondition != ConditionArray[i]));
		if (bCondition != ConditionArray[i]) return false;
	}

	return true;
}
