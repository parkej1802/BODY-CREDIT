﻿#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CWeaponComponent.h"
#include "CNoxAnimInstance.generated.h"

UCLASS()
class BODYCREDIT_API UCNoxAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

protected:
	UPROPERTY(BlueprintReadOnly)
	class ACNox* OwnerCharacter;

	UPROPERTY()
	class UCWeaponComponent* Weapon;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	EWeaponType WeaponType = EWeaponType::MAX;

protected:
	UPROPERTY(BlueprintReadOnly)
	float Direction;

	UPROPERTY(BlueprintReadOnly)
	float Speed;

	UPROPERTY(BlueprintReadOnly)
	bool bFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	bool bAiming;

private:
	FRotator PrevRotation;

};
