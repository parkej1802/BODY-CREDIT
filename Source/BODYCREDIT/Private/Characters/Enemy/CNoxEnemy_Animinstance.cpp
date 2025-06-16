#include "Characters/Enemy/CNoxEnemy_Animinstance.h"

#include "Characters/Enemy/CNox_MedicAndroid.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"
#include "Characters/Enemy/CNox_Zero.h"
#include "Utilities/CLog.h"
#include "Global.h"

#pragma region Update Animation
void UCNoxEnemy_Animinstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	IdleIdx = FMath::RandRange(0, 8);
}

void UCNoxEnemy_Animinstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwnerEnemy) return;

	{
		FVector velo = OwnerEnemy->GetVelocity();
		Speed = velo.Size();

		velo.Z = 0;
		velo.Normalize();
		const float Dot = FVector::DotProduct(OwnerEnemy->GetActorForwardVector(), velo);
		const float CrossZ = FVector::CrossProduct(OwnerEnemy->GetActorForwardVector(), velo).Z;
		DeltaYaw=FMath::RadiansToDegrees( FMath::Atan2( CrossZ, Dot ) );
	}
	
	if (loopCheck) // laser Beam (Medic)
	{
		float Elapsed = GetWorld()->GetTimeSeconds() - LoopStartTime;
		// CLog::Print(FString::Printf(TEXT("Elapsed : %.2f"), Elapsed));
		if (Elapsed >= MaxLoopDuration)
		{
			loopCheck = false;
			Cast<ACNox_MemoryCollectorAI>(OwnerEnemy)->BeamAttackEnd();
		}
	}
}
#pragma endregion

#pragma region Attacking
void UCNoxEnemy_Animinstance::PlayAttackMontage()
{
	if (OwnerEnemy->IsA(ACNox_MedicAndroid::StaticClass()) ||
		OwnerEnemy->IsA(ACNox_Zero::StaticClass()))
	{
		if (AttackMontage) OwnerEnemy->PlayAnimMontage(AttackMontage, 1.0f);
	}
	else if (OwnerEnemy->IsA(ACNox_MemoryCollectorAI::StaticClass()))
	{
		if (Attack1Montage)
		{
			AttackCombo = 1;
			OwnerEnemy->PlayAnimMontage(Attack1Montage, 1.0f);
		}
	}
}

bool UCNoxEnemy_Animinstance::IsAttacking() const
{
	if (OwnerEnemy->IsA(ACNox_MedicAndroid::StaticClass()) || OwnerEnemy->IsA(ACNox_Zero::StaticClass()))
	{
		if (AttackMontage && Montage_IsPlaying(AttackMontage)) return true;
		else return false;
	}
	else if (OwnerEnemy->IsA(ACNox_MemoryCollectorAI::StaticClass()))
	{
		UAnimMontage* curMontage = OwnerEnemy->GetCurrentMontage();
		if (curMontage == Attack1Montage || curMontage == Attack2Montage || curMontage == Attack3Montage || curMontage
			== Attack4Montage)
			return true;
		else return false;
	}

	return false;
}

void UCNoxEnemy_Animinstance::AnimNotify_EnableRAttack()
{
	OwnerEnemy->AttackCollision(true, true);
}

void UCNoxEnemy_Animinstance::AnimNotify_EnableLAttack()
{
	OwnerEnemy->AttackCollision(true, false);
}

void UCNoxEnemy_Animinstance::AnimNotify_EndAttack()
{
	OwnerEnemy->AttackCollision(false);
}
#pragma endregion

#pragma region Grenade
void UCNoxEnemy_Animinstance::PlayGrenadeMontage() const
{
	OwnerEnemy->PlayAnimMontage(GrenadeMontage, 1.0f);
}

bool UCNoxEnemy_Animinstance::IsPlayingGrenade() const
{
	return Montage_IsPlaying(GrenadeMontage);
}

void UCNoxEnemy_Animinstance::AnimNotify_Grenade() const
{
	Cast<ACNox_MedicAndroid>(OwnerEnemy)->LaunchElectricGrenade();
}
#pragma endregion

#pragma region Heal
void UCNoxEnemy_Animinstance::PlayShieldMontage(const bool bInShieldStart) const
{
	if (bInShieldStart)
		OwnerEnemy->PlayAnimMontage(ShieldMontage, 1.0f, ShieldStartSection);
	else
		OwnerEnemy->PlayAnimMontage(ShieldMontage, -.65f, ShieldEndSection);
}

bool UCNoxEnemy_Animinstance::IsShielding() const
{
	return Montage_IsPlaying(ShieldMontage);
}
#pragma endregion

#pragma region Beam
void UCNoxEnemy_Animinstance::PlayBeamAttack() const
{
	OwnerEnemy->PlayAnimMontage(BeamMontage, 1.0f);
}

void UCNoxEnemy_Animinstance::AnimNotify_BeamStart() const
{
	Cast<ACNox_MemoryCollectorAI>(OwnerEnemy)->BeamAttack();
}

void UCNoxEnemy_Animinstance::StopBeamAttack() const
{
	OwnerEnemy->StopAnimMontage(BeamMontage);
}

bool UCNoxEnemy_Animinstance::IsBeamAttacking() const
{
	return Montage_IsPlaying(BeamMontage);
}

void UCNoxEnemy_Animinstance::AnimNotify_UsingBeamTimeChecker()
{
	loopCheck = true;
	LoopStartTime = GetWorld()->GetTimeSeconds();
}
#pragma endregion

#pragma region Wave Pulse
void UCNoxEnemy_Animinstance::PlayWavePulse() const
{
	OwnerEnemy->PlayAnimMontage(WavePulseMontage, 1.0f);
}

void UCNoxEnemy_Animinstance::AnimNotify_WavePulseStart() const
{
	Cast<ACNox_MemoryCollectorAI>(OwnerEnemy)->PulseWaveAttack();
}

bool UCNoxEnemy_Animinstance::IsWavePulseAttacking() const
{
	return Montage_IsPlaying(WavePulseMontage);
}
#pragma endregion

#pragma region Range Attack (Memory Collector)
void UCNoxEnemy_Animinstance::AnimNotify_SaveAttack()
{
	UAnimMontage* tmpMontage = nullptr;
	switch (AttackCombo)
	{
	case 0:
		// tmpMontage = Attack1Montage;
		break;
	case 1:
		tmpMontage = Attack2Montage;
		break;
	case 2:
		tmpMontage = Attack3Montage;
		break;
	case 3:
		tmpMontage = Attack4Montage;
		break;
	default:
		break;
	}

	if (tmpMontage)
	{
		OwnerEnemy->PlayAnimMontage(tmpMontage, 1.0f);
		AttackCombo = (AttackCombo + 1) % 4;
	}
}

void UCNoxEnemy_Animinstance::AnimNotify_ResetCombo()
{
	AttackCombo = 0;
}

void UCNoxEnemy_Animinstance::AnimNotify_RangeAttack()
{
	UAnimMontage* curMontage = OwnerEnemy->GetCurrentMontage();
	if (curMontage == Attack1Montage || curMontage == Attack3Montage)
	{
		// 오른손
		Cast<ACNox_MemoryCollectorAI>(OwnerEnemy)->StartRangeAttack(true);
	}
	else if (curMontage == Attack2Montage || curMontage == Attack4Montage)
	{
		// 왼손
		Cast<ACNox_MemoryCollectorAI>(OwnerEnemy)->StartRangeAttack(false);
	}
}
#pragma endregion

#pragma region Hit
void UCNoxEnemy_Animinstance::PlayHitMontage(const int32 sectionIdx)
{
	OwnerEnemy->PlayAnimMontage(HitMontage, 1, FName(FString::FromInt(sectionIdx)));
}

bool UCNoxEnemy_Animinstance::IsHitting() const
{
	return Montage_IsPlaying(HitMontage);
}
#pragma endregion

#pragma region Die
void UCNoxEnemy_Animinstance::PlayDieMontage(const int32 sectionIdx)
{
	if (DieMontage) OwnerEnemy->PlayAnimMontage(DieMontage, 1.0f);
}

void UCNoxEnemy_Animinstance::AnimNotify_PlayAttackSound()
{
	if (OwnerEnemy->IsA(ACNox_Zero::StaticClass()))
		OwnerEnemy->PlayAttackSound(FMath::RandRange(0, 4));
}
#pragma endregion
