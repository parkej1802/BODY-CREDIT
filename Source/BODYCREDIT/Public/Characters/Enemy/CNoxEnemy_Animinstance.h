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

#pragma region Init

private: // InIt
	UPROPERTY(BlueprintReadOnly, Category=Anim, meta=(AllowPrivateAccess=true))
	int32 IdleIdx = 0;

	virtual void NativeBeginPlay() override;
#pragma endregion

#pragma region Update Animation

private:
	UPROPERTY()
	ACNox_EBase* OwnerEnemy;
	UPROPERTY(BlueprintReadOnly, Category=Anim, meta=(AllowPrivateAccess=true))
	float Speed;
	UPROPERTY(BlueprintReadOnly, Category=Anim, meta=(AllowPrivateAccess=true))
	float DeltaYaw;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
#pragma endregion

#pragma region Set Owner

public:
	void SetEnemy(ACNox_EBase* InOwnerEnemy) { OwnerEnemy = InOwnerEnemy; }
#pragma endregion

#pragma region Montage

public:
	// Common
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* IdleMontage;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* HitMontage;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* DieMontage;
	// Zero, Medic
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* AttackMontage;
	// Medic
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* GrenadeMontage;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* ShieldMontage;
	// Memory Collector
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
#pragma endregion

#pragma region Attacking

public:
	void PlayAttackMontage();
	bool IsAttacking() const;

private:
	UFUNCTION()
	void AnimNotify_EnableRAttack();
	UFUNCTION()
	void AnimNotify_EnableLAttack();
	UFUNCTION()
	void AnimNotify_EndAttack();
#pragma endregion

#pragma region Grenade

public:
	void PlayGrenadeMontage() const;
	bool IsPlayingGrenade() const;

private:
	UFUNCTION()
	void AnimNotify_Grenade() const;
#pragma endregion

#pragma region Heal

private:
	const FName ShieldStartSection = "ShieldStart";
	const FName ShieldEndSection = "ShieldEnd";

public:
	void PlayShieldMontage(const bool bInShieldStart) const;
	bool IsShielding() const;
#pragma endregion

#pragma region Beam

public:
	void PlayBeamAttack() const;
	void StopBeamAttack() const;
	bool IsBeamAttacking() const;

private:
	bool loopCheck = false;
	double LoopStartTime;
	UPROPERTY(EditDefaultsOnly)
	float MaxLoopDuration = 5.f;

	UFUNCTION()
	void AnimNotify_UsingBeamTimeChecker();
	UFUNCTION()
	void AnimNotify_BeamStart() const;
#pragma endregion

#pragma region Wave Pulse

public:
	void PlayWavePulse() const;
	bool IsWavePulseAttacking() const;

private:
	UFUNCTION()
	void AnimNotify_WavePulseStart() const;
#pragma endregion

#pragma region Range Attack (Memory Collector)

private:
	int8 AttackCombo = 0;

	UFUNCTION()
	void AnimNotify_SaveAttack();
	UFUNCTION()
	void AnimNotify_ResetCombo();
	UFUNCTION()
	void AnimNotify_RangeAttack();
#pragma endregion

#pragma region Hit

public:
	void PlayHitMontage(const int32 sectionIdx = 1);
	bool IsHitting() const;
#pragma endregion

#pragma region Die

public:
	void PlayDieMontage(const int32 sectionIdx = 1);
#pragma endregion
};
