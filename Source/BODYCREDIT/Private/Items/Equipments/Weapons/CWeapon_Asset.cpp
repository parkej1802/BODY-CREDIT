#include "Items/Equipments/Weapons/CWeapon_Asset.h"
#include "Global.h"
#include "Characters/CNox.h"
#include "Items/Equipments/Weapons/CWeapon_Data.h"
#include "Items/Equipments/Weapons/CWeapon_Attachment.h"
#include "Items/Equipments/Weapons/CWeapon_Equipment.h"
#include "Items/Equipments/Weapons/CWeapon_DoAction.h"
#include "Items/Equipments/Weapons/CWeapon_SubAction.h"

UCWeapon_Asset::UCWeapon_Asset()
{
	WeaponAttachmentClass = ACWeapon_Attachment::StaticClass();
	WeaponEquipmentClass = UCWeapon_Equipment::StaticClass();

}

void UCWeapon_Asset::BeginPlay(ACNox* InOwner, UCWeapon_Data** OutWeaponData)
{
	ACWeapon_Attachment* attachment = nullptr;
	if (!!WeaponAttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		attachment = InOwner->GetWorld()->SpawnActor<ACWeapon_Attachment>(WeaponAttachmentClass, params);
	}

	UCWeapon_Equipment* equipment = nullptr;
	if (!!WeaponEquipmentClass)
	{
		equipment = NewObject<UCWeapon_Equipment>(this, WeaponEquipmentClass);
		equipment->BeginPlay(InOwner, WeaponEquipmentData);

		if (!!attachment)
		{
			equipment->OnWeaponEquipmentBeginEquip.AddDynamic(attachment, &ACWeapon_Attachment::OnBeginEquip);
			equipment->OnWeaponEquipmentUnequip.AddDynamic(attachment, &ACWeapon_Attachment::OnUnequip);
		}
	}

	UCWeapon_DoAction* doAction = nullptr;
	if (!!WeaponDoActionClass)
	{
		doAction = NewObject<UCWeapon_DoAction>(this, WeaponDoActionClass);
		doAction->BeginPlay(attachment, equipment, InOwner, WeaponDoActionDatas, WeaponHitDatas);

		if (!!attachment)
		{
			attachment->OnWeaponAttachmentBeginCollision.AddDynamic(doAction, &UCWeapon_DoAction::OnWeaponAttachmentBeginCollision);
			attachment->OnWeaponAttachmentEndCollision.AddDynamic(doAction, &UCWeapon_DoAction::OnWeaponAttachmentEndCollision);

			attachment->OnWeaponAttachmentBeginOverlap.AddDynamic(doAction, &UCWeapon_DoAction::OnWeaponAttachmentBeginOverlap);
			attachment->OnWeaponAttachmentEndOverlap.AddDynamic(doAction, &UCWeapon_DoAction::OnWeaponAttachmentEndOverlap);
		}

		if (!!equipment)
		{
			equipment->OnWeaponEquipmentBeginEquip.AddDynamic(doAction, &UCWeapon_DoAction::OnBeginEquip);
			equipment->OnWeaponEquipmentUnequip.AddDynamic(doAction, &UCWeapon_DoAction::OnUnequip);
		}
	}

	UCWeapon_SubAction* subAction = nullptr;
	if (!!WeaponSubActionClass)
	{
		subAction = NewObject<UCWeapon_SubAction>(this, WeaponSubActionClass);
		subAction->BeginPlay(InOwner, attachment, doAction);
	}

	*OutWeaponData = NewObject<UCWeapon_Data>();
	(*OutWeaponData)->WeaponAttachment = attachment;
	(*OutWeaponData)->WeaponEquipment = equipment;
	(*OutWeaponData)->WeaponDoAction = doAction;
	(*OutWeaponData)->WeaponSubAction = subAction;

}
