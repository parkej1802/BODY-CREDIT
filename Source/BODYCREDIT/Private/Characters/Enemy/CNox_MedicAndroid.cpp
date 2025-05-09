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
	CHelpers::GetAssetDynamic(&(EnemyAnim->GrenadeMontage),
	                          TEXT("/Game/Characters/Enemy/Anim/MedicAnim/AM_Grenade.AM_Grenade"));
	CHelpers::GetAssetDynamic(&(EnemyAnim->ShieldMontage),
	                          TEXT("/Game/Characters/Enemy/Anim/MedicAnim/AM_Shield.AM_Shield"));
	EnemyAnim->AnimNotify_PlayIdleMontage();
}

void ACNox_MedicAndroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsEquipShield)
	{
		CurShieldTime += DeltaTime;
		if (CurShieldTime >= ShieldInterval)
		{
			CurShieldTime = 0.f;
			bIsEquipShield = false;
			EnemyAnim->PlayShieldMontage(bIsEquipShield);
		}
	}
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

void ACNox_MedicAndroid::HandleEquipShield(const bool bInEquipShield)
{
	EnemyAnim->PlayShieldMontage(bInEquipShield);
}

float ACNox_MedicAndroid::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                     class AController* EventInstigator, AActor* DamageCauser)
{
	if (!bIsEquipShield)
	{
		bIsEquipShield = true;
		EnemyAnim->PlayShieldMontage(bIsEquipShield);
	}
	else
		CurShieldTime = 0.f;
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
