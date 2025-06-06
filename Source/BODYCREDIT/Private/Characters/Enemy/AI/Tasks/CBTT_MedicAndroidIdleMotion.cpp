#include "Characters/Enemy/AI/Tasks/CBTT_MedicAndroidIdleMotion.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_MedicAndroid.h"

EBTNodeResult::Type UCBTT_MedicAndroidIdleMotion::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ACNox_MedicAndroid* MyEnemy = Cast<
		ACNox_MedicAndroid>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
