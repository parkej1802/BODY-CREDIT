// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CNox_EBase.h"
#include "Animation/AnimInstance.h"
#include "CNoxEnemy_Animinstance.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCNoxEnemy_Animinstance : public UAnimInstance
{
	GENERATED_BODY()

private:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY()
	ACNox_EBase* OwnerEnemy;

public: // Medic Android
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* IdleMontage;

public:
	FORCEINLINE void SetEnemy(ACNox_EBase* InOwnerEnemy) { OwnerEnemy = InOwnerEnemy; }

public:
	UFUNCTION()
	void AnimNotify_PlayIdleMontage();
};
