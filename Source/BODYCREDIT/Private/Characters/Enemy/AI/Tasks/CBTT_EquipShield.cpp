// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_EquipShield.h"

#include "Characters/Enemy/CNox_MedicAndroid.h"
#include "BehaviorTree/BlackboardComponent.h"

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
		bGrenadeEnd = OwnerComp.GetBlackboardComponent()->GetValueAsBool(GrenadeEndKey);
		bCanUseShield = OwnerComp.GetBlackboardComponent()->GetValueAsBool(CanUseShieldKey);
		bHitDamage = OwnerComp.GetBlackboardComponent()->GetValueAsBool(HitDamageKey);
		bIsEquipShield = OwnerComp.GetBlackboardComponent()->GetValueAsBool(EquipShieldKey);
		bIsBrokenShield = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BrokenShieldKey);

		if (!bGrenadeEnd) return EBTNodeResult::Succeeded;

		if (!bIsEquipShield && bCanUseShield && bHitDamage)
			MyEnemy->HandleEquipShield(true);
		else if (bIsEquipShield && bIsBrokenShield)
			MyEnemy->HandleEquipShield(false);

		OwnerComp.GetBlackboardComponent()->SetValueAsBool(EquipShieldKey, !bIsEquipShield);

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
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ShieldKey, false);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
