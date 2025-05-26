// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_WavePulse.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"

UCBTT_WavePulse::UCBTT_WavePulse()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_WavePulse::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ACNox_MemoryCollectorAI* MyEnemy = Cast<
		ACNox_MemoryCollectorAI>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		MyEnemy->ShutPulseWave();
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}

void UCBTT_WavePulse::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (ACNox_MemoryCollectorAI* MyEnemy = Cast<
		ACNox_MemoryCollectorAI>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		if (!MyEnemy->IsPlayPulseWave())
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("WavePulseCoolTime"), 0.f);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(FName("WavePulseCoolTime"), 0.f);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
