// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CNox_EBase.h"
#include "CNox_Zero.generated.h"

/**
 * 폐기된 실험체
 */
UCLASS()
class BODYCREDIT_API ACNox_Zero : public ACNox_EBase
{
	GENERATED_BODY()

public:
	ACNox_Zero();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	class ACPatrolRoute* NearPatrolRoute;

public:
	class ACPatrolRoute* GetNearPatrolRoute();
	void GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed,
							 float& OutNewAccelSpeed) override;
	void SetDesiredRotation(const FRotator& InDesiredRotation);
};
