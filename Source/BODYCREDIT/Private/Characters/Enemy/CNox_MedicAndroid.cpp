#include "Characters/Enemy/CNox_MedicAndroid.h"
#include "Global.h"
#include "Characters/Enemy/CNoxEnemy_Animinstance.h"
#include "Characters/Enemy/AttackActor/CElectricGrenade.h"
#include "Components/Enemy/CNoxEnemyHPComponent.h"

#pragma region Init
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

	ConstructorHelpers::FClassFinder<ACElectricGrenade> GrenadeClass(
		TEXT("/Game/Characters/Enemy/AttackActor/BP_ElectircGrenade.BP_ElectircGrenade_C"));
	if (GrenadeClass.Succeeded())
		ElectricGrenadeCls = GrenadeClass.Class;
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

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	ElectricGrenade = GetWorld()->SpawnActor<ACElectricGrenade>(ElectricGrenadeCls, this->GetActorLocation(),
	                                                            this->GetActorRotation(),
	                                                            SpawnParams);
}

void ACNox_MedicAndroid::SetPerceptionInfo()
{
	Super::SetPerceptionInfo();
	RetentionTime = 0.f;
}

void ACNox_MedicAndroid::GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed,
											 float& OutNewAccelSpeed)
{
	switch (InMovementSpeed)
	{
	case EEnemyMovementSpeed::Idle:
		OutNewSpeed = 0.f;
		OutNewAccelSpeed = 0.f;
		break;
	case EEnemyMovementSpeed::Walking:
		OutNewSpeed = 280.f;
		OutNewAccelSpeed = 450.f;
		break;
	case EEnemyMovementSpeed::Sprinting:
		OutNewSpeed = 320.f;
		OutNewAccelSpeed = 1024.f;
		break;
	default:
		break;
	}
}
#pragma endregion

void ACNox_MedicAndroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region Take Damage
float ACNox_MedicAndroid::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
									 AController* EventInstigator, AActor* DamageCauser)
{
	if (!GetTarget())
		if (ACNox* player = Cast<ACNox>(DamageCauser->GetOwner())) SetTarget(player);
	if (IsShielding()) return 0.f;
	HPComp->TakeDamage(DamageAmount);
	return DamageAmount;
}
#pragma endregion

#pragma region Heal
bool ACNox_MedicAndroid::IsLowHealth()
{
	return HPComp->GetHealthPercent() <= HealStdValue;
}

void ACNox_MedicAndroid::HandleEquipShield(const bool bInEquipShield)
{
	EnemyAnim->PlayShieldMontage(bInEquipShield);
}

bool ACNox_MedicAndroid::IsShielding() const
{
	return EnemyAnim->IsShielding();
}
#pragma endregion

#pragma region Electric Grenade
void ACNox_MedicAndroid::HandleElectricGrenade()
{
	EnemyAnim->PlayGrenadeMontage();
}

bool ACNox_MedicAndroid::IsPlayingGrenade() const
{
	return EnemyAnim->IsPlayingGrenade();
}
#pragma endregion

#pragma region Launch Grenade
void ACNox_MedicAndroid::SuggestProjectileVelocityWithLimit(FVector& OutVelocity, const FVector& StartLocation,
                                                            const FVector& TargetLocation, float MaxSpeed,
                                                            float GravityZ)
{
	const FVector Delta = TargetLocation - StartLocation;
	FVector DeltaXY = FVector(Delta.X, Delta.Y, 0.f);
	float HorizontalDistance = DeltaXY.Size();
	float DeltaZ = Delta.Z;

	float ArcHeight = FMath::Clamp(HorizontalDistance * .2f, 100.f, 300.f);

	float Vz = FMath::Sqrt(2 * FMath::Abs(GravityZ) * ArcHeight);

	float TimeUp = Vz / FMath::Abs(GravityZ);
	float TimeDown = FMath::Sqrt(FMath::Max(2 * (ArcHeight - DeltaZ), 0.f) / FMath::Abs(GravityZ));
	float TotalTime = TimeUp + TimeDown;

	// 최소 시간 보정 (너무 가까울 때 처리)
	if (TotalTime <= 0.f) TotalTime = 0.1f;

	FVector DirXY = DeltaXY.GetSafeNormal();
	float Vxy = HorizontalDistance / TotalTime;

	FVector Velocity = DirXY * Vxy + FVector(0, 0, Vz);
	float InitialSpeed = Velocity.Size();

	if (InitialSpeed > MaxSpeed)
	{
		float Scale = MaxSpeed / InitialSpeed;
		Velocity *= Scale;
	}

	OutVelocity = Velocity;
}

void ACNox_MedicAndroid::LaunchElectricGrenade()
{
	FVector startLoc = GetMesh()->GetSocketLocation(FName("GrenadeSocket"));
	FVector targetLoc = Target->GetActorLocation();
	FVector outVelocity;
	SuggestProjectileVelocityWithLimit(outVelocity, this->GetActorLocation(), targetLoc);
	if (ElectricGrenade) ElectricGrenade->InitializeGrenade(startLoc, targetLoc, outVelocity);
}
#pragma endregion
