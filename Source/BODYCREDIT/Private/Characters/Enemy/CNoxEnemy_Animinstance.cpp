// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNoxEnemy_Animinstance.h"

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
}

void UCNoxEnemy_Animinstance::AnimNotify_PlayIdleMontage()
{
	OwnerEnemy->PlayAnimMontage(IdleMontage, 1.0f);
}

void UCNoxEnemy_Animinstance::PlayGrenadeMontage()
{
	OwnerEnemy->SetGrenadeEnded(false);
	OwnerEnemy->PlayAnimMontage(GrenadeMontage, 1.0f);
}

void UCNoxEnemy_Animinstance::PlayShieldMontage(const bool bInShieldStart)
{
	BehaviorComponent->SetShieldEnded(false);
	bUsingShield = bInShieldStart;
	
	if (bInShieldStart)
		OwnerEnemy->PlayAnimMontage(ShieldMontage, 1.0f, ShieldStartSection);
	else
		OwnerEnemy->PlayAnimMontage(ShieldMontage, -1.0f, ShieldEndSection);
}
