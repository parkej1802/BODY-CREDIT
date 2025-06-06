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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
							 class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* AttackComp_l;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* AttackComp_r;

	UFUNCTION()
	void OnAttackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                   const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere)
	class ACPatrolRoute* NearPatrolRoute;

public:
	class ACPatrolRoute* GetNearPatrolRoute();
	virtual void GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed,
	                                 float& OutNewAccelSpeed) override;

public:
	virtual void AttackCollision(bool bOn, bool IsRightHand = true) override;
};
