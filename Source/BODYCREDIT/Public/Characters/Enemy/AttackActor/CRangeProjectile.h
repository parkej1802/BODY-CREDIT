#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CRangeProjectile.generated.h"

UCLASS()
class BODYCREDIT_API ACRangeProjectile : public AActor
{
	GENERATED_BODY()

public:
	ACRangeProjectile();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void InitializeProjectile(FVector InStartLocation, class ACNox* InTargetActor);
	void SetCollisionEnabled(bool bEnabled);

private:
	UPROPERTY()
	class ACNox_MemoryCollectorAI* OwnerAI;
	FVector StartLocation;
	UPROPERTY()
	class ACNox* Target;
	FVector CurrentVelocity;
	float Speed = 800.0f;
	float HomingStrength = 2.0f; // 높을수록 더 잘 따라감
	float MaxTurnRateDegPerSec = 60.0f; // 너무 휘지 않도록 제한

private:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* ProjectileFxComp;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	               int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
