// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_EquipShield.h"

#include "Characters/Enemy/CNox_MedicAndroid.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Utilities/CLog.h"

UCBTT_EquipShield::UCBTT_EquipShield()
{
	bNotifyTick = true;
	bCreateNodeInstance = true; //  Enemy 마다 각각의 인스턴스 생성
}

EBTNodeResult::Type UCBTT_EquipShield::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ACNox_MedicAndroid* MyEnemy = Cast<
		ACNox_MedicAndroid>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
	{
		// 쉴드를 들고있으면 내리고, 내리고있으면 들게한다
		bIsEquipShield = OwnerComp.GetBlackboardComponent()->GetValueAsBool(EquipShieldKey);

		// CLog::Log(FString::Printf(TEXT("[UCBTT_EquipShield::ExecuteTask] bIsEquipShield Reverse: %d"), !bIsEquipShield));
		MyEnemy->HandleEquipShield(!bIsEquipShield);

		return EBTNodeResult::InProgress;
	}
	else
		return EBTNodeResult::Failed;
}

void UCBTT_EquipShield::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetBlackboardComponent()->GetValueAsBool(ShieldKey))
	{
		// 애니메이션 종료 플레그 초기화
		// Anim - MontageEnded에서 true로 변경해줌
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ShieldKey, false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		CurHealTime += DeltaSeconds;
		if (CurHealTime >= HealTime)
		{
			CurHealTime = 0.f;

			if (ACNox_MedicAndroid* MyEnemy = Cast<
				ACNox_MedicAndroid>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor"))))
			{
				
			}
		}
	}
}
