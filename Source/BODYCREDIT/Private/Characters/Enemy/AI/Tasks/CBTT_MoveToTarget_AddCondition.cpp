#include "Characters/Enemy/AI/Tasks/CBTT_MoveToTarget_AddCondition.h"

#include "AIController.h"
#include "Global.h"

UCBTT_MoveToTarget_AddCondition::UCBTT_MoveToTarget_AddCondition()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_MoveToTarget_AddCondition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	float distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(DirectionRadiusKey.SelectedKeyName);
	AcceptableRadius = distance;

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

void UCBTT_MoveToTarget_AddCondition::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	float CurrentCoolDown = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(ConditionKey.SelectedKeyName);
	bool chkCondition = OwnerComp.GetBlackboardComponent()->GetValueAsBool(TargetInRadiusCheckKey.SelectedKeyName);
	// CLog::Log(FString::Printf(TEXT("chkCondition : %d, CurrentCoolDown >= ConditionValue : %d"), chkCondition,
	//                           CurrentCoolDown >= ConditionValue));
	if (chkCondition && CurrentCoolDown >= ConditionValue)
	{
		AAIController* AIController = OwnerComp.GetAIOwner();
		if (AIController)
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(PlayGrenadeKey, true);
			// 이동을 중단합니다
			// AIController->StopMovement();
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
