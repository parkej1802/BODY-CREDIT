#include "Items/Equipments/Weapons/CWeapon_Structures.h"
#include "Global.h"
#include "Characters/CNox.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Animation/AnimMontage.h"

/************************
*  Weapon DoActionData  *
************************/

void FWeaponDoActionData::DoAction(ACNox* InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);

	if (!!movement)
	{
		if (bFixedCamera)
			movement->EnableFixedCamera();

		if (bCanMove == false)
			movement->Stop();
	}

	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);

}

void FWeaponDoActionData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation);

	CHelpers::PlayEffect(InWorld, Effect, transform);

}

void FWeaponDoActionData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);

	CHelpers::PlayEffect(InWorld, Effect, transform);

}

/*******************
*  Weapon HitData  *
*******************/

void FWeaponHitData::SendDamage(ACNox* InAttacker, AActor* InAttackCauser, ACNox* InOther, const FHitResult HitResult)
{
	FActionDamageEvent e;
	e.WeaponHitData = this;
	e.HitInfo = HitResult;
	e.ShotDirection = (InOther->GetActorLocation() - InAttacker->GetActorLocation()).GetSafeNormal();

	InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);

}

void FWeaponHitData::PlayMontage(ACNox* InOwner)
{
	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);

}

void FWeaponHitData::PlayHitStop(UWorld* InWorld)
{
	CheckTrue(FMath::IsNearlyZero(StopTime));

	TArray<ACharacter*> characters;
	for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);

		if (!!character)
		{
			character->CustomTimeDilation = 1e-3f;

			characters.Add(character);
		}
	}

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
		{
			for (ACharacter* character : characters)
				character->CustomTimeDilation = 1;
		});

	FTimerHandle timerHandle;
	InWorld->GetTimerManager().SetTimer(timerHandle, timerDelegate, StopTime, false);

}

void FWeaponHitData::PlaySoundWave(ACNox* InOwner)
{
	CheckNull(Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);

}

void FWeaponHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(EffectLocation);
	transform.SetScale3D(EffectScale);
	transform.AddToTranslation(InLocation);

	CHelpers::PlayEffect(InWorld, Effect, transform);

}

void FWeaponHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation)
{
	CheckNull(Effect);

	FTransform transform;
	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);

	CHelpers::PlayEffect(InWorld, Effect, transform);

}
