#include "Characters/Enemy/CNox_MedicAndroid.h"
#include "Global.h"
#include "Characters/Enemy/CNoxEnemy_Animinstance.h"
#include "Components/Enemy/CNoxEnemyHPComponent.h"

ACNox_MedicAndroid::ACNox_MedicAndroid()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tmpMesh(TEXT(
		"/Game/Assets/Sci_Fi_Characters_Pack/Mesh/Sci_Fi_Character_02/SK_Sci_Fi_Character_02_Full.SK_Sci_Fi_Character_02_Full"));
	if (tmpMesh.Succeeded())
		GetMesh()->SetSkeletalMesh(tmpMesh.Object);

	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -110), FRotator(0, -90, 0));
	GetMesh()->SetRelativeScale3D(FVector(1.55, 1.35, 1.3));

	GetCapsuleComponent()->SetCapsuleHalfHeight(110.f);
	GetCapsuleComponent()->SetCapsuleRadius(42.f);

	ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClass(
		TEXT("/Game/Characters/Enemy/Anim/MedicAnim/ABP_Medic.ABP_Medic_C"));
	if (AnimInstanceClass.Succeeded())
		GetMesh()->SetAnimInstanceClass(AnimInstanceClass.Class);

	EnemyType = EEnemyType::MedicAndroid;
	SetPerceptionInfo();
}

void ACNox_MedicAndroid::BeginPlay()
{
	Super::BeginPlay();

	CHelpers::GetAssetDynamic(&(EnemyAnim->IdleMontage), TEXT("/Game/Assets/MedicAnim/LocomotionAnim/AM_Idle.AM_Idle"));
	CHelpers::GetAssetDynamic(&(EnemyAnim->GrenadeMontage),
	                          TEXT("/Game/Assets/MedicAnim/AttackAnim/AM_Grenade.AM_Grenade"));
	CHelpers::GetAssetDynamic(&(EnemyAnim->ShieldMontage),
	                          TEXT("/Game/Assets/MedicAnim/HealAnim/AM_Shield.AM_Shield"));
	CHelpers::GetAssetDynamic(&(EnemyAnim->AttackMontage),
	                          TEXT("/Game/Assets/MedicAnim/AttackAnim/AM_Attack.AM_Attack"));
}

void ACNox_MedicAndroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

void ACNox_MedicAndroid::GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed,
                                             float& OutNewAccelSpeed)
{
	OutNewSpeed = 0.f;
	OutNewAccelSpeed = 600.f;

	switch (InMovementSpeed)
	{
	case EEnemyMovementSpeed::Idle:
		OutNewSpeed = 0.f;
		OutNewAccelSpeed = 0.f;
		break;
	case EEnemyMovementSpeed::Walking:
		OutNewSpeed = 400.f;
		OutNewAccelSpeed = 450.f;
		break;
	case EEnemyMovementSpeed::Jogging:
		OutNewSpeed = 0.f;
		break;
	case EEnemyMovementSpeed::Sprinting:
		OutNewSpeed = 500.f;
		break;
	}
}

void ACNox_MedicAndroid::HandleIdleMotion()
{
	EnemyAnim->AnimNotify_PlayIdleMontage();
}

void ACNox_MedicAndroid::HandleElectricGrenade()
{
	EnemyAnim->PlayGrenadeMontage();
}

void ACNox_MedicAndroid::HandleEquipShield(const bool bInEquipShield)
{
	// CLog::Log(FString::Printf(TEXT("[ACNox_MedicAndroid::HandleEquipShield] bInEquipShield: %d"), bInEquipShield));

	bIsEquipShield = bInEquipShield;
	EnemyAnim->PlayShieldMontage(bInEquipShield);
	BehaviorComp->SetEquipShield(bInEquipShield);
}

float ACNox_MedicAndroid::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                     class AController* EventInstigator, AActor* DamageCauser)
{
	bool bIsDamageShield = false;
	if (bIsEquipShield)
		bIsDamageShield = true;

	// 데미지 처리 추가
	bool bIsShieldCrash = false;
	HPComp->TakeDamage(DamageAmount, bIsDamageShield, bIsShieldCrash);

	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

bool ACNox_MedicAndroid::IsLowHealth()
{
	return HPComp->GetHealthPercent() <= HealStdValue;
}

void ACNox_MedicAndroid::SetHealFlag(bool bHealFlag)
{
	BehaviorComp->SetHealFlag(bHealFlag);
}

void ACNox_MedicAndroid::HealEnd()
{
	EnemyAnim->JumpShieldMontage();
}
