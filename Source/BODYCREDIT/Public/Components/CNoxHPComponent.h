// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CNoxHPComponent.generated.h"

/**
 *	Base Hp Component
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BODYCREDIT_API UCNoxHPComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCNoxHPComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/**
	*	Status Property - LHJ (2025.05.07)
	*/
	UPROPERTY(EditAnywhere, Category="Status")
	float MaxHealth{100};
	UPROPERTY(VisibleAnywhere, Category="Status")
	float Health;

	UPROPERTY(EditAnywhere, Category="Status")
	float MaxDefense{50};
	UPROPERTY(VisibleAnywhere, Category="Status")
	float Defense;

	bool bIsDead{false};

	UPROPERTY(EditAnywhere, Category="Status")
	float ShieldRegenTimeMax = 3.f;
	float CurShieldRegenTime;

public:
	/**
	 *	Common Virtual Function - LHJ (2025.05.07)
	 */
	virtual void TakeDamage(float Amount, bool ActiveShield, bool& OutIsShieldCrash);
	virtual void Die();

protected:
	virtual void InitStatus();

public:
	float GetHealthPercent() const { return Health / MaxHealth; }
};
