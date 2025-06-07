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
	virtual auto TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	                        AController* EventInstigator, AActor* DamageCauser) -> float override;
	virtual void GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed,
	                         float& OutNewAccelSpeed) override;

public: // Animation
	bool IsShielding() const;
	void HandleElectricGrenade();
	bool IsPlayingGrenade() const;

public: // Heal
	float CurShieldTime = 0.f;
	float ShieldInterval = 2.f;

	bool IsLowHealth();
	void HandleEquipShield(const bool bInEquipShield);

private:
	UPROPERTY(EditDefaultsOnly, Category=Heal)
	float HealStdValue = 0.4f;
	UPROPERTY(EditDefaultsOnly, Category=Heal)
	class UNiagaraSystem* HealEffectFactory = nullptr;
	UPROPERTY()
	class UNiagaraComponent* HealEffect = nullptr;

private: // Electric Grenade
	UPROPERTY(VisibleDefaultsOnly)
	TSubclassOf<class ACElectricGrenade> ElectricGrenadeCls;
	UPROPERTY(VisibleAnywhere)
	class ACElectricGrenade* ElectricGrenade;

	void SuggestProjectileVelocityWithLimit(FVector& OutVelocity,
	                                        const FVector& StartLocation,
	                                        const FVector& TargetLocation,
	                                        float MaxSpeed = 1500.f,
	                                        float GravityZ = -980.f
	);

public:
	void LaunchElectricGrenade();
};
