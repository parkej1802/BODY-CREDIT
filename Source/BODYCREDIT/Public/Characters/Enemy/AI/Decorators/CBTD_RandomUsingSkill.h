// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTD_RandomUsingSkill.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTD_RandomUsingSkill : public UBTDecorator
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector UseWavePulseKey;
	UPROPERTY(EditAnywhere, Category=Blackboard)
	FBlackboardKeySelector UseBeamAttackKey;
	UPROPERTY(EditAnywhere, Category=Blackboard)
	float WavePulsePercent = 20.f;
	UPROPERTY(EditAnywhere, Category=Blackboard)
	float BeamAttackPercent = 20.f;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	int32 GetRandomPercent() const;
};
