// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CNox.h"
#include "CNox_EBase.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	CCTV,
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
	float GetSightRadius() const {return SightRadius;}
	float GetLoseSightRadius() const {return LoseSightRadius;}
	float GetPeripheralVisionAngleDegrees() const {return PeripheralVisionAngleDegrees;}
	float GetRetentionTime() const {return RetentionTime;}
	float GetHearingRange() const {return HearingRange;}
	
protected:	// Status
	UPROPERTY(EditDefaultsOnly)
	EEnemyType EnemyType;

protected:	// Virtual Function
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
