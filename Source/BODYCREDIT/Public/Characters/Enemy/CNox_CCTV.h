// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "CNox_CCTV.generated.h"

/**
 * CCTV
 */
UCLASS()
class BODYCREDIT_API ACNox_CCTV : public ACNox_EBase
{
	GENERATED_BODY()

public:
	ACNox_CCTV();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

private:
	FRotator InitialRotation;

	UPROPERTY(EditAnywhere, Category = "CCTV")
	float MinYaw = -60.f;

	UPROPERTY(EditAnywhere, Category = "CCTV")
	float MaxYaw = 60.f;

	UPROPERTY(EditAnywhere, Category = "CCTV")
	float RotationSpeed = 30.f;

	UPROPERTY(EditAnywhere, Category = "CCTV")
	float PauseTimeAtEnds = 1.5f;

	bool bRotatingRight = true;
	float PauseTimer = 0.f;
	bool bIsPaused = false;

	float SumRotYaw = 0.f;
	
	void RotateCCTV(float DeltaTime);

private:
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* CCTVMesh;

	UFUNCTION()
	void BroadCastDetectPlayer(ACNox* DetectPlayer);	// 주변 적에게 정보 전달
};
