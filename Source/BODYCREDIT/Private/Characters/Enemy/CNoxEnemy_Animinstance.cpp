#include "Characters/Enemy/CNoxEnemy_Animinstance.h"

#include "Characters/Enemy/CNox_MedicAndroid.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"
#include "Characters/Enemy/CNox_Zero.h"
#include "Utilities/CLog.h"
#include "Global.h"

void UCNoxEnemy_Animinstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UCNoxEnemy_Animinstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	this->OnMontageEnded.AddDynamic(this, &UCNoxEnemy_Animinstance::OnAnimMontageEnded);
	IdleIdx = FMath::RandRange(0, 8);
}

void UCNoxEnemy_Animinstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwnerEnemy) return;

	FVector velo = OwnerEnemy->GetVelocity();
	Speed = velo.Size();

	velo.Z = 0;
	velo.Normalize();
	const float Dot = FVector::DotProduct(OwnerEnemy->GetActorForwardVector(), velo);
	const float CrossZ = FVector::CrossProduct(OwnerEnemy->GetActorForwardVector(), velo).Z;
	DeltaYaw=FMath::RadiansToDegrees( FMath::Atan2( CrossZ, Dot ) );

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
	// if (Montage == GrenadeMontage)
	// 	OwnerEnemy->SetGrenadeEnded(true);
	// else if (Montage == ShieldMontage)
	// 	BehaviorComponent->SetShieldEnded(true);
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

	// BehaviorComponent->SetShieldEnded(false);

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

void UCNoxEnemy_Animinstance::PlayHitMontage(const int32 sectionIdx)
{
	OwnerEnemy->PlayAnimMontage(HitMontage, sectionIdx);
}

bool UCNoxEnemy_Animinstance::IsHitting() const
{
	if (AttackMontage && Montage_IsPlaying(AttackMontage)) return true;

	return false;
}

void UCNoxEnemy_Animinstance::PlayDieMontage(const int32 sectionIdx)
{
	if (DieMontage) OwnerEnemy->PlayAnimMontage(DieMontage, 1.0f);
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
		// 왼손
		Cast<ACNox_MemoryCollectorAI>(OwnerEnemy)->StartRangeAttack(false);
	}
	else if (curMontage == Attack2Montage || curMontage == Attack4Montage)
	{
		// 오른손
		Cast<ACNox_MemoryCollectorAI>(OwnerEnemy)->StartRangeAttack(true);
	}
}

void UCNoxEnemy_Animinstance::AnimNotify_Grenade()
{
	Cast<ACNox_MedicAndroid>(OwnerEnemy)->LaunchElectricGrenade();
}
