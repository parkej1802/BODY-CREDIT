#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CGunComponent.h"
#include "CAnimInstance.generated.h"

UCLASS()
class BODYCREDIT_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Pitch;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Direction;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	EGunType WeaponType = EGunType::Max;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	bool bInAim;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	bool bUseIK;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	FVector LeftHandLocation;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
	void OnWeaponTypeChanged(EGunType InPrevType, EGunType InNewType);

private:
	class ACNox_Player* OwnerCharacter;
	class UCGunComponent* Weapon;

private:
	FRotator PrevRotation;
	
};
