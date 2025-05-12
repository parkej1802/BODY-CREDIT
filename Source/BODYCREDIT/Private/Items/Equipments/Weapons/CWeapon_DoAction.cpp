#include "Items/Equipments/Weapons/CWeapon_DoAction.h"
#include "Global.h"
#include "Characters/CNox.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

UCWeapon_DoAction::UCWeapon_DoAction()
{

}

void UCWeapon_DoAction::BeginPlay(ACWeapon_Attachment* InAttachment, UCWeapon_Equipment* InEquipment, ACNox* InOwner, const TArray<FWeaponDoActionData>& InDoActionDatas, const TArray<FWeaponHitData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;

}

void UCWeapon_DoAction::DoAction()
{
	bInAction = true;

	State->SetActionMode();

}

void UCWeapon_DoAction::Begin_DoAction()
{
	bBeginAction = true;

}

void UCWeapon_DoAction::End_DoAction()
{
	bInAction = false;
	bBeginAction = false;

	State->SetIdleMode();

	Movement->Move();
	Movement->DisableFixedCamera();

}
