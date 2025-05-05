// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "CNox_MemoryCollectorAI.generated.h"

/**
 * 기억 추적 AI
 */
UCLASS()
class BODYCREDIT_API ACNox_MemoryCollectorAI : public ACNox_EBase
{
	GENERATED_BODY()

public:
	ACNox_MemoryCollectorAI();

private:
	virtual void BeginPlay() override;
};
