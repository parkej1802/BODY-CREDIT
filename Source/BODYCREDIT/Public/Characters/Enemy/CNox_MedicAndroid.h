// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "CNox_MedicAndroid.generated.h"

/**
 * 의료 방어 의체
 */
UCLASS()
class BODYCREDIT_API ACNox_MedicAndroid : public ACNox_EBase
{
	GENERATED_BODY()

public:
	ACNox_MedicAndroid();

private:
	virtual void BeginPlay() override;

	virtual void SetPerceptionInfo() override;

public:
	virtual void SetTarget(class ACNox* InTarget) override;

public:
	float CurElectricGrenadeCoolTime = 0.f;
	float ElectricGrenadeCoolTimeMax = 3.f;

	void HandleElectricGrenade();

public:
	void HandleEquipShield(const bool bInEquipShield);
};
