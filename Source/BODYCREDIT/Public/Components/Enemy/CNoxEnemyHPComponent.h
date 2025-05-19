// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CNoxHPComponent.h"
#include "CNoxEnemyHPComponent.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCNoxEnemyHPComponent : public UCNoxHPComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class ACNox_EBase* OwnerEnemy;

private:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
public:
	void SetEnemy(class ACNox_EBase* InNewOwner);
	void HealHP(float InHealAmount);
};
