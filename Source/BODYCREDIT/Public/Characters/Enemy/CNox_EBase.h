// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CNox.h"
#include "CNox_EBase.generated.h"

/**
 * Enemy Base
 */
UCLASS()
class BODYCREDIT_API ACNox_EBase : public ACNox
{
	GENERATED_BODY()

public:
	ACNox_EBase();

private: // Sensing
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* Perception;

	UPROPERTY()
	class UAISenseConfig_Hearing* Hearing;

	UPROPERTY()
	class UAISenseConfig_Sight* Sight;

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
};
