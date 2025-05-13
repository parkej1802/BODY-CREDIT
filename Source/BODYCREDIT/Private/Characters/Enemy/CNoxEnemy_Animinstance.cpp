// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNoxEnemy_Animinstance.h"

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
	if (!Montage_IsPlaying(IdleMontage))
	{
		if (Speed >= KINDA_SMALL_NUMBER)
			PlaySlotAnimationAsDynamicMontage(IdleMontage, FName("UpperBody"));
		else
			PlaySlotAnimationAsDynamicMontage(IdleMontage, FName("FullBody"));
	}
	// OwnerEnemy->PlayAnimMontage(IdleMontage, 1.0f);
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
