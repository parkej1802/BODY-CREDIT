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
	virtual void Tick(float DeltaTime) override;
	virtual void SetPerceptionInfo() override;

public:
	virtual void SetTarget(class ACNox* InTarget) override;
	void GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed, float& OutNewAccelSpeed) override;

public:
	void HandleIdleMotion();

public:
	void HandleElectricGrenade();

public:
	bool bIsEquipShield = false;
	float CurShieldTime = 0.f;
	float ShieldInterval = 2.f;

	void HandleEquipShield(const bool bInEquipShield);

private:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	                         class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(EditDefaultsOnly, Category=Heal)
	float HealStdValue = 0.4f;
	
public:
	bool IsLowHealth();
	void SetHealFlag(bool bHealFlag);
	void HealEnd();
};
