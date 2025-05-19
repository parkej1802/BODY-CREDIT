// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Decorators/CBTD_CheckOtherFloor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/AI/Keys/CBBK_MemoryFragment.h"

bool UCBTD_CheckOtherFloor::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	auto enemy = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor")));
	if (!enemy) return false;

	uint8* RawData = OwnerComp.GetBlackboardComponent()->GetKeyRawData(MemoryTargetKey.SelectedKeyName);
	auto memory = UCBBK_MemoryFragment::GetValue(RawData);

	float dist = FMath::Abs(enemy->GetActorLocation().Z - memory.Location.Z);
	if (dist < Distance) return false;

	return true;	// true면 다른 층
}
