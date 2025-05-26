#include "Characters/Enemy/AI/Tasks/CBTT_BeamAttack.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"

UCBTT_BeamAttack::UCBTT_BeamAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_BeamAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ACNox_MemoryCollectorAI* MyEnemy = Cast<
		ACNox_MemoryCollectorAI>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		MyEnemy->ShutBeam();
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UCBTT_BeamAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (ACNox_MemoryCollectorAI* MyEnemy = Cast<
		ACNox_MemoryCollectorAI>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		if (!MyEnemy->IsPlayBeam())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("BeamCoolTime"), 0.f);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("BeamCoolTime"), 0.f);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
