// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CNox.h"
#include "CNox_EBase.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Cctv = 0,
	Zero,
	MedicAndroid,
	MemoryCollector,
};

/**
 * Enemy Base
 */
UCLASS()
class BODYCREDIT_API ACNox_EBase : public ACNox
{
	GENERATED_BODY()

public:
	ACNox_EBase();

protected: // Sensing Property
	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float SightRadius = 300.f;
	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float LoseSightRadius = 400.f;
	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float PeripheralVisionAngleDegrees = 45.f;
	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float RetentionTime = 2.f;

	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float HearingRange = 800.f;

public: // Get Sensing Function
	float GetSightRadius() const { return SightRadius; }
	float GetLoseSightRadius() const { return LoseSightRadius; }
	float GetPeripheralVisionAngleDegrees() const { return PeripheralVisionAngleDegrees; }
	float GetRetentionTime() const { return RetentionTime; }
	float GetHearingRange() const { return HearingRange; }

protected: // Set Sensing Function
	virtual void SetPerceptionInfo() {}

protected: // Status
	UPROPERTY(EditDefaultsOnly)
	EEnemyType EnemyType;

protected: // Virtual Function
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected: // Component
	UPROPERTY(VisibleDefaultsOnly)
	class UCNox_BehaviorComponent* BehaviorComp;
	UPROPERTY(VisibleDefaultsOnly)
	class UCNoxEnemy_Animinstance* EnemyAnim;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCNoxEnemyHPComponent* HPComp;

public:
	bool bUseBehaviorTree = true;

private:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	class UBehaviorTree* BehaviorTree;

public:
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }

public:
	// CCTV에서는 Blackboard에 세팅은 안하고 주변 Enemy에게만 전달한다.
	virtual void SetTarget(ACNox* InTarget);

protected:
	UPROPERTY(EditDefaultsOnly, Category=Health)
	float HealAmount=10.f;
	
public:
	void HealHP();

public:	// Medic Android
	void SetGrenadeEnded(bool InbEndedAnim);
};
