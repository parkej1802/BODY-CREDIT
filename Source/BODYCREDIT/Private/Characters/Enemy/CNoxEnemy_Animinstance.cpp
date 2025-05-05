// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNoxEnemy_Animinstance.h"

void UCNoxEnemy_Animinstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UCNoxEnemy_Animinstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UCNoxEnemy_Animinstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}

void UCNoxEnemy_Animinstance::AnimNotify_PlayIdleMontage()
{
	OwnerEnemy->PlayAnimMontage(IdleMontage, 1.0f);
}
