// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNox_MedicAndroid.h"
#include "Global.h"
#include "Characters/Enemy/CNoxEnemy_Animinstance.h"

ACNox_MedicAndroid::ACNox_MedicAndroid()
{
	EnemyType = EEnemyType::MedicAndroid;
	SetPerceptionInfo();
}

void ACNox_MedicAndroid::BeginPlay()
{
	Super::BeginPlay();

	CHelpers::GetAssetDynamic(&(EnemyAnim->IdleMontage), TEXT("/Game/Characters/Enemy/Anim/MedicAnim/AM_Idle.AM_Idle"));
	CHelpers::GetAssetDynamic(&(EnemyAnim->GrenadeMontage), TEXT("/Game/Characters/Enemy/Anim/MedicAnim/AM_Grenade.AM_Grenade"));
	EnemyAnim->AnimNotify_PlayIdleMontage();
}

void ACNox_MedicAndroid::SetPerceptionInfo()
{
	Super::SetPerceptionInfo();

	RetentionTime = 0.f;
}

void ACNox_MedicAndroid::SetTarget(class ACNox* InTarget)
{
	Super::SetTarget(InTarget);
	EnemyAnim->StopAllMontages(0.25f);
}

void ACNox_MedicAndroid::HandleElectricGrenade()
{
	EnemyAnim->PlayGrenadeMontage();
}
