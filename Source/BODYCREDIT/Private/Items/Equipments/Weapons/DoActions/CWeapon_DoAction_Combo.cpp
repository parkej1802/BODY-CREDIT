#include "Items/Equipments/Weapons/DoActions/CWeapon_DoAction_Combo.h"
#include "Global.h"
#include "Characters/CNox.h"
#include "Characters/CNox_Runner.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"

void UCWeapon_DoAction_Combo::DoAction()
{
	CheckTrue(DoActionDatas.Num() < 1);

	if (bEnable)
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(State->IsIdleMode());

	// Super::DoAction();
	// DoActionDatas[Index].DoAction(OwnerCharacter);

	if (CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter)->GetWeaponType() == EWeaponType::BOW)
	{
		if (CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter)->IsBowSubActionActive())
		{
			Super::DoAction();
			DoActionDatas[Index].DoAction(OwnerCharacter);
		}
		else DoActionDatas[0].DoAction(OwnerCharacter);
	}
	else if (CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter)->GetWeaponType() == EWeaponType::KATANA)
	{
		Super::DoAction();
		DoActionDatas[Index].DoAction(OwnerCharacter);
	}
	
}

void UCWeapon_DoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);

	bExist = false;
	DoActionDatas[++Index].DoAction(OwnerCharacter);

}

void UCWeapon_DoAction_Combo::End_DoAction()
{
	Super::End_DoAction();

	Index = 0;

}

void UCWeapon_DoAction_Combo::OnWeaponAttachmentBeginOverlap(ACNox* InAttacker, AActor* InAttackCauser, ACNox* InOther)
{
	Super::OnWeaponAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);
	CheckNull(InOther);

	//CLog::Log(InOther->GetName());

	//FActionDamageEvent e;
	//e.HitData = &HitDatas[0];

	//InOther->TakeDamage(e.HitData->Power, e, InAttacker->GetController(), InAttackCauser);

	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);

	Hitted.AddUnique(InOther);
	if (HitVFX)
		UGameplayStatics::SpawnEmitterAtLocation(World, HitVFX, InOther->GetActorLocation());

	CheckTrue(HitDatas.Num() - 1 < Index);
	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);

}

void UCWeapon_DoAction_Combo::OnWeaponAttachmentEndCollision()
{
	Super::OnWeaponAttachmentEndCollision();

	float angle = -2.0f;
	ACharacter* candidate = nullptr;

	for (ACharacter* hitted : Hitted)
	{
		FVector direction = hitted->GetActorLocation() - OwnerCharacter->GetActorLocation();
		direction = direction.GetSafeNormal2D();

		FVector forward = FQuat(OwnerCharacter->GetActorRotation()).GetForwardVector();

		float dot = FVector::DotProduct(direction, forward);
		if (dot >= angle)
		{
			angle = dot;
			candidate = hitted;
		}
	}

	if (!!candidate)
	{
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), candidate->GetActorLocation());
		FRotator target = FRotator(0, rotator.Yaw, 0);

		if (ACNox_Runner* runner = Cast<ACNox_Runner>(OwnerCharacter))
		{
			runner->TargetControlRotation = FRotator(0.f, rotator.Yaw, 0.f);
			runner->bShouldRotateToTarget = true;
		}
		
		// AController* controller = OwnerCharacter->GetController<AController>();
		// controller->SetControlRotation(target);
	}

	Hitted.Empty();

}
