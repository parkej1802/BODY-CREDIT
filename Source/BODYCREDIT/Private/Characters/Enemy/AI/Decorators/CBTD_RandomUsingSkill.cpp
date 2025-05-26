// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Decorators/CBTD_RandomUsingSkill.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"

bool UCBTD_RandomUsingSkill::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	int32 rndPercemt = GetRandomPercent();

	if (UseWavePulseKey.SelectedKeyType != UBlackboardKeyType_Bool::StaticClass())
	{
		// Beam Attack 확률 계산
		if (rndPercemt <= BeamAttackPercent)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(UseBeamAttackKey.SelectedKeyName, true);
			return true;
		}
	}
	else if (UseBeamAttackKey.SelectedKeyType != UBlackboardKeyType_Bool::StaticClass())
	{
		// Wave Pluse 확률 계산
		if (rndPercemt <= WavePulsePercent)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(UseWavePulseKey.SelectedKeyName, true);
			return true;
		}
	}
	else if (UseWavePulseKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass() && UseBeamAttackKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass())
	{
		// 둘 다 확률 계산
		if (rndPercemt <= BeamAttackPercent)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(UseBeamAttackKey.SelectedKeyName, true);
			return true;
		}
		else if (rndPercemt <= BeamAttackPercent + WavePulsePercent)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(UseWavePulseKey.SelectedKeyName, true);
			return true;
		}
	}

	return false;
}

int32 UCBTD_RandomUsingSkill::GetRandomPercent() const
{
	return FMath::RandRange(1, 100);
}
