// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNoxEnemy_Animinstance.h"

#include "Characters/Enemy/CNox_MedicAndroid.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"
#include "Utilities/CLog.h"

void UCNoxEnemy_Animinstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UCNoxEnemy_Animinstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	this->OnMontageEnded.AddDynamic(this, &UCNoxEnemy_Animinstance::OnAnimMontageEnded);
}

void UCNoxEnemy_Animinstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwnerEnemy) return;

	Speed = OwnerEnemy->GetVelocity().Size();

	if (loopCheck)
	{
		float Elapsed = GetWorld()->GetTimeSeconds() - LoopStartTime;
		CLog::Print(FString::Printf(TEXT("Elapsed : %.2f"), Elapsed));
		if (Elapsed >= MaxLoopDuration)
		{
			loopCheck = false;
			Cast<ACNox_MemoryCollectorAI>(OwnerEnemy)->BeamAttackEnd();
		}
	}
}

void UCNoxEnemy_Animinstance::OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == GrenadeMontage)
		OwnerEnemy->SetGrenadeEnded(true);
	else if (Montage == ShieldMontage)
		BehaviorComponent->SetShieldEnded(true);
}

void UCNoxEnemy_Animinstance::AnimNotify_PlayIdleMontage()
{
	OwnerEnemy->PlayAnimMontage(IdleMontage, 1.0f);
}

void UCNoxEnemy_Animinstance::AnimNotify_DistanceToPlayer()
{
	if (!OwnerEnemy->IsPlayerInDistance())
		Montage_Stop(0.25f, AttackMontage);
}

void UCNoxEnemy_Animinstance::PlayGrenadeMontage()
{
	OwnerEnemy->SetGrenadeEnded(false);
	OwnerEnemy->PlayAnimMontage(GrenadeMontage, 1.0f);
}

void UCNoxEnemy_Animinstance::PlayShieldMontage(const bool bInShieldStart)
{
	// CLog::Log(FString::Printf(TEXT("bUsingShield: %d"), bInShieldStart));

	BehaviorComponent->SetShieldEnded(false);

	if (bInShieldStart)
		OwnerEnemy->PlayAnimMontage(ShieldMontage, 1.0f, ShieldStartSection);
	else
		OwnerEnemy->PlayAnimMontage(ShieldMontage, -.65f, ShieldEndSection);
}

void UCNoxEnemy_Animinstance::JumpShieldMontage()
{
	if (ShieldMontage && Montage_IsPlaying(ShieldMontage))
		Montage_JumpToSection(FName("ShieldEnd"), ShieldMontage);
}

void UCNoxEnemy_Animinstance::PlayAttackMontage()
{
	if (OwnerEnemy->IsA(ACNox_MedicAndroid::StaticClass()))
	{
		if (AttackMontage)
		{
			OwnerEnemy->PlayAnimMontage(AttackMontage, 1.0f);
		}
	}
}

bool UCNoxEnemy_Animinstance::IsAttacking() const
{
	if (OwnerEnemy->IsA(ACNox_MedicAndroid::StaticClass()))
	{
		if (AttackMontage && Montage_IsPlaying(AttackMontage))
		{
			return true;
		}
	}

	return false;
}

void UCNoxEnemy_Animinstance::PlayBeamAttack()
{
	OwnerEnemy->PlayAnimMontage(BeamMontage, 1.0f);
}

void UCNoxEnemy_Animinstance::StopBeamAttack()
{
	OwnerEnemy->StopAnimMontage(BeamMontage);
}

bool UCNoxEnemy_Animinstance::IsBeamAttacking() const
{
	return Montage_IsPlaying(BeamMontage);
}

void UCNoxEnemy_Animinstance::AnimNotify_BeamStart()
{
	Cast<ACNox_MemoryCollectorAI>(OwnerEnemy)->BeamAttack();
}

void UCNoxEnemy_Animinstance::AnimNotify_UsingBeamTimeChecker()
{
	loopCheck = true;
	LoopStartTime = GetWorld()->GetTimeSeconds();
}

void UCNoxEnemy_Animinstance::PlayWavePulse()
{
	OwnerEnemy->PlayAnimMontage(WavePulseMontage, 1.0f);
}

bool UCNoxEnemy_Animinstance::IsWavePulseAttacking() const
{
	return Montage_IsPlaying(WavePulseMontage);
}

void UCNoxEnemy_Animinstance::AnimNotify_WavePulseStart()
{
	Cast<ACNox_MemoryCollectorAI>(OwnerEnemy)->PulseWaveAttack();
}
