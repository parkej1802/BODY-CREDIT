#include "Characters/Enemy/AI/Tasks/CBTT_MoveToTarget.h"

EBTNodeResult::Type UCBTT_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	float distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(DirectionRadiusKey.SelectedKeyName);
	AcceptableRadius = distance;

	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
