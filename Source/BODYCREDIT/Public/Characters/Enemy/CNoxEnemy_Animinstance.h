// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CNox_EBase.h"
#include "Animation/AnimInstance.h"
#include "Components/Enemy/CNox_BehaviorComponent.h"
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
	UFUNCTION()
	void OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
private:
	UPROPERTY()
	ACNox_EBase* OwnerEnemy;
	UPROPERTY()
	UCNox_BehaviorComponent* BehaviorComponent;

	UPROPERTY(BlueprintReadOnly, Category=Anim, meta=(AllowPrivateAccess=true))
	float Speed;

public: // Medic Android
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* IdleMontage;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* GrenadeMontage;

public:
	FORCEINLINE void SetEnemy(ACNox_EBase* InOwnerEnemy) { OwnerEnemy = InOwnerEnemy; }
	FORCEINLINE void SetBT(UCNox_BehaviorComponent* InBT) { BehaviorComponent = InBT; }

public:
	UFUNCTION()
	void AnimNotify_PlayIdleMontage();

public:
	UPROPERTY(BlueprintReadWrite, Category = "Anim")
	bool bUsingShield = false;
	
	void PlayGrenadeMontage();
};
