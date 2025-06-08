#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "Characters/CNox.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Items/Equipments/Weapons/CWeapon_Asset.h"
#include "Items/Equipments/Weapons/CWeapon_Data.h"
#include "Items/Equipments/Weapons/CWeapon_Attachment.h"
#include "Items/Equipments/Weapons/CWeapon_Equipment.h"
#include "Items/Equipments/Weapons/CWeapon_DoAction.h"
#include "Items/Equipments/Weapons/CWeapon_SubAction.h"
#include "Components/CZoomComponent.h"

UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CHelpers::GetAsset<UInputAction>(&IA_Action, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Action.IA_Action'"));
	CHelpers::GetAsset<UInputAction>(&IA_SubAction, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_SubAction.IA_SubAction'"));

	CHelpers::GetAsset<UInputAction>(&IA_Bow, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Bow.IA_Bow'"));
	CHelpers::GetAsset<UInputAction>(&IA_Rifle, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Rifle.IA_Rifle'"));
	CHelpers::GetAsset<UInputAction>(&IA_Katana, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Katana.IA_Katana'"));

}

void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int32 i = 0; i < (int32)EWeaponType::MAX; i++)
	{
		if (!!DataAssets[i])
			DataAssets[i]->BeginPlay(OwnerCharacter, &Datas[i]);
	}

}

void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!!GetDoAction())
		GetDoAction()->Tick(DeltaTime);

	if (!!GetSubAction())
		GetSubAction()->Tick(DeltaTime);

}

bool UCWeaponComponent::IsIdleMode()
{
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsIdleMode();

}

ACWeapon_Attachment* UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetAttachment();

}

UCWeapon_Equipment* UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetEquipment();

}

UCWeapon_DoAction* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetDoAction();

}

UCWeapon_SubAction* UCWeaponComponent::GetSubAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetSubAction();

}

void UCWeaponComponent::SetKatanaMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::KATANA);

}

void UCWeaponComponent::SetBowMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::BOW);

}

void UCWeaponComponent::SetRifleMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::RIFLE);

}

void UCWeaponComponent::SetUnarmedMode()
{
	GetEquipment()->Unequip();

	ChangeType(EWeaponType::MAX);

}

void UCWeaponComponent::DoAction()
{
	if (!!GetDoAction())
		GetDoAction()->DoAction();

}

void UCWeaponComponent::SubAction_Pressed()
{
	if (IsBowMode())
	{
		if (UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter))
		{
			if (movement->IsSprint())
			{
				if (movement->IsSlide())
				{
					CHelpers::GetComponent<UCZoomComponent>(OwnerCharacter)->SetComponentTickEnabled(false);
				}
				else return;
			}
		}
	}

	if (!!GetSubAction())
		GetSubAction()->Pressed();

}

void UCWeaponComponent::SubAction_Released()
{
	if (IsBowMode())
		CHelpers::GetComponent<UCZoomComponent>(OwnerCharacter)->SetComponentTickEnabled(true);

	if (!!GetSubAction())
		GetSubAction()->Released();

}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();

		return;
	}
	else if (IsUnarmedMode() == false)
	{
		GetEquipment()->Unequip();
	}

	if (!!Datas[(int32)InType])
	{
		Datas[(int32)InType]->GetEquipment()->Equip();

		ChangeType(InType);
	}

}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = Type;
	Type = InType;

	if (OnWeaponTypeChange.IsBound())
		OnWeaponTypeChange.Broadcast(prevType, InType);

}

void UCWeaponComponent::BindInput(UEnhancedInputComponent* InEnhancedInputComponent)
{
	InEnhancedInputComponent->BindAction(IA_Bow, ETriggerEvent::Started, this, &UCWeaponComponent::SetBowMode);
	InEnhancedInputComponent->BindAction(IA_Rifle, ETriggerEvent::Started, this, &UCWeaponComponent::SetRifleMode);
	InEnhancedInputComponent->BindAction(IA_Katana, ETriggerEvent::Started, this, &UCWeaponComponent::SetKatanaMode);

	InEnhancedInputComponent->BindAction(IA_Action, ETriggerEvent::Started, this, &UCWeaponComponent::DoAction);

	InEnhancedInputComponent->BindAction(IA_SubAction, ETriggerEvent::Started, this, &UCWeaponComponent::SubAction_Pressed);
	InEnhancedInputComponent->BindAction(IA_SubAction, ETriggerEvent::Completed, this, &UCWeaponComponent::SubAction_Released);

}
