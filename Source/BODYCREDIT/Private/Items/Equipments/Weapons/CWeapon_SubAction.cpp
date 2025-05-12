#include "Items/Equipments/Weapons/CWeapon_SubAction.h"
#include "Global.h"
#include "Characters/CNox.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CapsuleComponent.h"

UCWeapon_SubAction::UCWeapon_SubAction()
{

}

void UCWeapon_SubAction::BeginPlay(ACNox* InOwner, ACWeapon_Attachment* InAttachment, UCWeapon_DoAction* InDoAction)
{
	Owner = InOwner;
	Attachment = InAttachment;
	DoAction = InDoAction;

	State = CHelpers::GetComponent<UCStateComponent>(Owner);
	Movement = CHelpers::GetComponent<UCMovementComponent>(Owner);

}

void UCWeapon_SubAction::Pressed()
{
	bInAction = true;

}

void UCWeapon_SubAction::Released()
{
	bInAction = false;

}
