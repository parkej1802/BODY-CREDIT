#include "Items/Equipments/Weapons/CWeapon_Equipment.h"
#include "Global.h"
#include "Characters/CNox.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

void UCWeapon_Equipment::BeginPlay(ACNox* InOwner, const FWeaponEquipmentData& InData)
{
	OwnerCharacter = InOwner;
	Data = InData;

	State = CHelpers::GetComponent<UCStateComponent>(InOwner);
	Movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);

}

void UCWeapon_Equipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (OnWeaponEquipmentEquip.IsBound())
		OnWeaponEquipmentEquip.Broadcast();

	if (Data.bCanMove == false)
		Movement->Stop();

	if (Data.bUseControlRotation)
		Movement->EnableControlRotation();

	if (!!Data.Montage)
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRate);
	else
	{
		Begin_Equip();
		End_Equip();
	}

}

void UCWeapon_Equipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;

	if (OnWeaponEquipmentBeginEquip.IsBound())
		OnWeaponEquipmentBeginEquip.Broadcast();

}

void UCWeapon_Equipment::End_Equip_Implementation()
{
	bBeginEquip = false;
	bEquipped = true;

	Movement->Move();
	State->SetIdleMode();

}

void UCWeapon_Equipment::Unequip_Implementation()
{
	bEquipped = false;
	//Movement->DisableControlRotation();

	if (OnWeaponEquipmentUnequip.IsBound())
		OnWeaponEquipmentUnequip.Broadcast();

}
