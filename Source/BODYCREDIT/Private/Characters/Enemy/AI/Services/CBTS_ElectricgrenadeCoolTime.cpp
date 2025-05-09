// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Services/CBTS_ElectricgrenadeCoolTime.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_MedicAndroid.h"

void UCBTS_ElectricgrenadeCoolTime::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	bool bCanUseElectricGrenade=OwnerComp.GetBlackboardComponent()->GetValueAsBool(ElectricGrenadeCoolTimeKey);
	if (auto MedicAI = Cast<ACNox_MedicAndroid>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		if (bCanUseElectricGrenade) return;

		MedicAI->CurElectricGrenadeCoolTime += DeltaSeconds;
		if (MedicAI->CurElectricGrenadeCoolTime >= MedicAI->ElectricGrenadeCoolTimeMax)
		{
			MedicAI->CurElectricGrenadeCoolTime = 0.f;
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(ElectricGrenadeCoolTimeKey, true);
		}
	}
}
