#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CElectricGrenade.generated.h"

UCLASS()
class BODYCREDIT_API ACElectricGrenade : public AActor
{
	GENERATED_BODY()

public:
	ACElectricGrenade();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* RootComp;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileComp;
	UPROPERTY(EditAnywhere, Category=FX)
	class UNiagaraComponent* FlashFX;

	FVector TargetLocation;
	float TriggerRadius = 100.f;
	bool bExploded = false;

	void Init(bool bInit);
	void UseFX(bool bUse);
	void Explode();

public:
	void InitializeGrenade(const FVector& InStartLocation, const FVector& InTargetLocation, const FVector& InVelocity,
	                       float InMaxSpeed = 1500);
};
