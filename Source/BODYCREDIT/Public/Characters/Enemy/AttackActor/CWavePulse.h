#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWavePulse.generated.h"

UCLASS()
class BODYCREDIT_API ACWavePulse : public AActor
{
	GENERATED_BODY()
	
public:	
	ACWavePulse();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* SphereComp;
	UPROPERTY(EditDefaultsOnly)
	class ACNox_EBase* OwnerAI;

public:
	void StartWave();
	
	UFUNCTION()
	void HandleWaveProgress(float Value);

	UFUNCTION()
	void OnWaveEnd();

	UPROPERTY()
	class UTimelineComponent* WaveTimeline;

	UPROPERTY(EditAnywhere)
	UCurveFloat* ScaleCurve;

	UPROPERTY(EditAnywhere)
	float MaxScale = 50.0f;

	UPROPERTY(EditAnywhere)
	FRotator MaxRotation = FRotator(45.f, 90.f, 45.f); // Pitch, Yaw, Roll 모두 포함
	UPROPERTY()
	TSet<AActor*> DamagedActors;
};
