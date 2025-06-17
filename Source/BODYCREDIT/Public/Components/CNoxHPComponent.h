// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CNoxHPComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, CurrentStamina, float, MaxStamina);

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

public:
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

	UPROPERTY(EditAnywhere, Category="Status")
	float MovementSpeed = 500;

	UPROPERTY(EditAnywhere, Category="Status")
	float Strength = 50.f;

	UPROPERTY(EditAnywhere, Category="Status")
	float Weight = 60.f;

	UPROPERTY(EditAnywhere, Category = "Status")
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, Category="Status")
	float Stamina = 0;

	UPROPERTY(EditAnywhere, Category="Status")
	float Humanity = 100.f;

public:
	/**
	 *	Common Virtual Function - LHJ (2025.05.07)
	 */
	virtual void TakeDamage(float Amount, bool ActiveShield, bool& OutIsShieldCrash); // For Enemy
	virtual void TakeDamage(float Amount); // For Player
	virtual void Die();
	void SetStatus(float newHP, float newDefense);

	UFUNCTION(BlueprintCallable)
	bool IsDead() const { return bIsDead; }

	// Widget Bind Function for RunnerUI
	// Health
	FOnHealthChanged OnHealthChanged;

	void SetHealth(float InNewHealth);

	float GetCurrentHealth() const { return Health; }
	float GetMaxHealth() const { return MaxHealth; }

	// Stamina
	FOnStaminaChanged OnStaminaChanged;

	void SetStamina(float InNewStamina);

	float GetCurrentStamina() const { return Stamina; }
	float GetMaxStamina() const { return MaxStamina; }

public:
	virtual void InitStatus();

public:
	float GetHealthPercent() const { return Health / MaxHealth; }

private:
	class ACNox_Runner* OwnerCharacter;

};
