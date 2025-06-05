#pragma once

#include "CoreMinimal.h"
#include "CNox_EBase.h"
#include "Animation/AnimInstance.h"
#include "CNoxEnemy_Animinstance.generated.h"

/**
 * Enemy AnimInstance
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

	UPROPERTY(BlueprintReadOnly, Category=Anim, meta=(AllowPrivateAccess=true))
	float Speed;

	// ==================================================================================
	// Medic Android
public:
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* IdleMontage;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* GrenadeMontage;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* ShieldMontage;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* AttackMontage;

	const FName ShieldStartSection = "ShieldStart";
	const FName ShieldEndSection = "ShieldEnd";

public:
	FORCEINLINE void SetEnemy(ACNox_EBase* InOwnerEnemy) { OwnerEnemy = InOwnerEnemy; }

public:
	UFUNCTION()
	void AnimNotify_PlayIdleMontage();

private:
	UFUNCTION()
	void AnimNotify_DistanceToPlayer();

public:
	void PlayGrenadeMontage();

public:
	void PlayShieldMontage(const bool bInShieldStart);
	void JumpShieldMontage();
	void PlayAttackMontage();
	bool IsAttacking() const;

public:
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* Attack1Montage;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* Attack2Montage;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* Attack3Montage;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* Attack4Montage;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* BeamMontage;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* WavePulseMontage;

public:
	void PlayBeamAttack();
	void StopBeamAttack();
	bool IsBeamAttacking() const;

private:
	UFUNCTION()
	void AnimNotify_BeamStart();

public:
	void PlayWavePulse();
	bool IsWavePulseAttacking() const;

private:
	UFUNCTION()
	void AnimNotify_WavePulseStart();

private:
	bool loopCheck = false;
	double LoopStartTime;
	UPROPERTY(EditDefaultsOnly)
	float MaxLoopDuration = 5.f;

	UFUNCTION()
	void AnimNotify_UsingBeamTimeChecker();

private:
	int8 AttackCombo = 0;

	UFUNCTION()
	void AnimNotify_SaveAttack();
	UFUNCTION()
	void AnimNotify_ResetCombo();
	UFUNCTION()
	void AnimNotify_RangeAttack();

private: //Zero
	UPROPERTY(BlueprintReadOnly, Category=Anim, meta=(AllowPrivateAccess=true))
	int32 IdleIdx = 0;
	UPROPERTY(BlueprintReadOnly, Category=Anim, meta=(AllowPrivateAccess=true))
	FRotator DesiredRotation = FRotator::ZeroRotator;
	UPROPERTY(BlueprintReadOnly, Category=Anim, meta=(AllowPrivateAccess=true))
	float DeltaYaw;

private:
	UFUNCTION()
	void AnimNotify_Grenade();
};
