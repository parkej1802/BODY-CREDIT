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
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileComp;

	FVector TargetLocation;
	float TriggerRadius = 100.f;

	void Init(bool bInit);
	void Explode();

public:
	void InitializeGrenade(const FVector& InStartLocation, const FVector& InTargetLocation, const FVector& InVelocity,
	                       float InMaxSpeed = 1500);
};
