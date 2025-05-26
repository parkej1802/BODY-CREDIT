// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_PlayRndSkill.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UCBTT_PlayRndSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (SkillCoolTimes.Num() < SkillCoolTimeKeys.Num()) return EBTNodeResult::Failed;

	TArray<int32> ValidSkillIndices;

	// 1. 쿨타임 지난 스킬 찾기
	for (int32 i = 0; i < SkillCoolTimeKeys.Num(); ++i)
	{
		const FName Key = SkillCoolTimeKeys[i].SelectedKeyName;
		const float CurrentTime = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(Key);
		if (CurrentTime >= SkillCoolTimes[i])
		{
			ValidSkillIndices.Add(i);
		}
	}

	if (ValidSkillIndices.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	// 2. 랜덤값으로 가중치 비교
	int32 RandomRoll = FMath::RandRange(1, 100);
	int32 SkillWeightSum = 0;
	for (int32 Index : ValidSkillIndices)
	{
		SkillWeightSum += SkillWeights[Index];
		if (RandomRoll <= SkillWeightSum)
		{
			// 3. 스킬 실행 - 여기에 원하는 Blackboard 값 설정 가능
			UE_LOG(LogTemp, Log, TEXT("Selected Skill Index: %d"), Index);

			// 선택된 스킬 인덱스를 Blackboard에 저장
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(TEXT("SelectedSkillIndex"), Index);

			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}
