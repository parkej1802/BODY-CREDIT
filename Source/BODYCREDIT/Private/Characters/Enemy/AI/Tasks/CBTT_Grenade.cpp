// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_Grenade.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_MedicAndroid.h"
#include "Utilities/CLog.h"

UCBTT_Grenade::UCBTT_Grenade()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_Grenade::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ACNox_MedicAndroid* MyEnemy = Cast<
		ACNox_MedicAndroid>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		// CLog::Log(FString::Printf(TEXT("GrenadeCanUse: %d"), OwnerComp.GetBlackboardComponent()->GetValueAsBool(GrenadeCanUseKey)));
		MyEnemy->HandleElectricGrenade(); // 애니메이션 실행
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(GrenadeCanUseKey, false); // 감전탄 쿨타임 실행
		return EBTNodeResult::InProgress;
	}
	else
		return EBTNodeResult::Failed;
}

void UCBTT_Grenade::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(GrenadeKey))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
