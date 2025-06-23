#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/AddOns/CAddOn.h"
#include "CAddOn_Arrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FProjectileHit, class AActor*, InCauser, class ACNox*, InOtherCharacter, const struct FHitResult&, HitResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileEndPlay, class ACAddOn_Arrow*, InDestroyer);

UCLASS()
class BODYCREDIT_API ACAddOn_Arrow : public ACAddOn
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class UParticleSystem* ArrowVanishFX;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class USoundBase* ArrowVanishSFX;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
	float LifeSpanAfterCollision = 1;

private:
	UPROPERTY(VisibleDefaultsOnly)
	class UCapsuleComponent* Capsule;

private:
	UPROPERTY(VisibleDefaultsOnly)
	class UProjectileMovementComponent* Projectile;

public:
	FORCEINLINE void AddIgnoreActor(AActor* InActor) { Ignores.Add(InActor); }

public:
	ACAddOn_Arrow();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void Shoot(const FVector& InForward);

	UProjectileMovementComponent* GetProjectileMovement() const { return Projectile; }

private:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	FProjectileHit OnHit;
	FProjectileEndPlay OnEndPlay;

private:
	TArray<AActor*> Ignores;

};
