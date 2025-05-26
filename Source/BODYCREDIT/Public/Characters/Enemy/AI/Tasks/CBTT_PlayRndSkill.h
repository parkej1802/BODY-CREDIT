// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_PlayRndSkill.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_PlayRndSkill : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	TArray<FBlackboardKeySelector> SkillCoolTimeKeys;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	TArray<float> SkillCoolTimes;
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	TArray<float> SkillWeights;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
