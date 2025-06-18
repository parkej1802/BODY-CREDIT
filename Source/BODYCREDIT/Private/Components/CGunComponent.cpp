#include "Components/CGunComponent.h"
#include "Global.h"
#include "Characters/CNox_Runner.h"
#include "Weapons/CWeapon.h"
#include "Components/CGunComponent.h"
// #include "Blueprint/UserWidget.h"
// #include "Widgets/CUserWidget_HUD.h"

UCGunComponent::UCGunComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// CHelpers::GetClass<UCUserWidget_HUD>(&HUDClass, "WidgetBlueprint'/Game/Widgets/WB_HUD.WB_HUD_C'");
}

void UCGunComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACNox_Runner>(GetOwner());
	CheckNull(Owner);


	FActorSpawnParameters params;
	params.Owner = Owner;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (TSubclassOf<ACWeapon> weaponClass : WeaponClasses)
	{
		if (!!weaponClass)
		{
			ACWeapon* weapon = Owner->GetWorld()->SpawnActor<ACWeapon>(weaponClass, params);
			Weapons.Add(weapon);
		}
	}

	//if (!!HUDClass)
	//{
	//	HUD = CreateWidget<UCUserWidget_HUD, APlayerController>(Owner->GetController<APlayerController>(), HUDClass);
	//	HUD->AddToViewport();
	//	HUD->SetVisibility(ESlateVisibility::Hidden);
	//}

	OnWeaponAim_Arms_Begin.AddDynamic(this, &UCGunComponent::On_Begin_Aim);
	OnWeaponAim_Arms_End.AddDynamic(this, &UCGunComponent::On_Begin_End);
}

void UCGunComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if (!!HUD)
	// {
	// 	if (!!GetCurrWeapon())
	// 		GetCurrWeapon()->IsAutoFire() ? HUD->OnAutoFire() : HUD->OffAutoFire();
	// 	else
	// 		HUD->OffAutoFire();
	//
	// 	if (!!GetCurrWeapon())
	// 	{
	// 		uint8 currCount = GetCurrWeapon()->GetCurrMagazineCount();
	// 		uint8 maxCount = GetCurrWeapon()->GetMaxMagazineCount();
	//
	// 		HUD->UpdateMagazine(currCount, maxCount);
	//
	// 		HUD->UpdateWeaponType(Type);
	// 	}
	// }
}

ACWeapon * UCGunComponent::GetCurrWeapon()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);

	return Weapons[(int32)Type];
}

void UCGunComponent::SetUnarmedMode()
{
	CheckFalse(GetCurrWeapon()->CanUnequip());

	GetCurrWeapon()->Unequip();
	ChangeType(EGunType::Max);

	// if(!!HUD)
	// 	HUD->SetVisibility(ESlateVisibility::Hidden);
}

void UCGunComponent::SetARMode()
{
	SetMode(EGunType::AR);
}

void UCGunComponent::SetAK47Mode()
{
	SetMode(EGunType::AK47);
}

void UCGunComponent::SetPistolMode()
{
	SetMode(EGunType::Pistol);
}

void UCGunComponent::SetMode(EGunType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();

		return;
	}
	else if (IsUnarmedMode() == false)
	{
		CheckFalse(GetCurrWeapon()->CanUnequip());

		GetCurrWeapon()->Unequip();
	}


	CheckNull(Weapons[(int32)InType]);
	CheckFalse(Weapons[(int32)InType]->CanEquip());
	
	Weapons[(int32)InType]->Equip();

	ChangeType(InType);

	// if (!!HUD)
	// 	HUD->SetVisibility(ESlateVisibility::Visible);
}

void UCGunComponent::ChangeType(EGunType InType)
{
	EGunType type = Type;
	Type = InType;

	if (OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(type, InType);
}

void UCGunComponent::Begin_Equip()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->Begin_Equip();
}

void UCGunComponent::End_Equip()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->End_Equip();
}

void UCGunComponent::Begin_Aim()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->Begin_Aim();
}

void UCGunComponent::End_Aim()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->End_Aim();
}

void UCGunComponent::Begin_Fire()
{
	CheckNull(GetCurrWeapon());
	CheckFalse(GetCurrWeapon()->CanFire());

	GetCurrWeapon()->Begin_Fire();
}

void UCGunComponent::End_Fire()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->End_Fire();
}

bool UCGunComponent::IsInAim()
{
	CheckNullResult(GetCurrWeapon(), false);

	return GetCurrWeapon()->IsInAim();
}

FVector UCGunComponent::GetLeftHandLocation()
{
	CheckNullResult(GetCurrWeapon(), FVector::ZeroVector);

	return GetCurrWeapon()->GetLeftHandLocation();
}

FTransform UCGunComponent::GetLeftHandTransform()
{
	CheckNullResult(GetCurrWeapon(), FTransform());

	return GetCurrWeapon()->GetArmsLeftHandTransform();
}

void UCGunComponent::ToggleAutoFire()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->ToggleAutoFire();
}

void UCGunComponent::Reload()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->Reload();
}

void UCGunComponent::Eject_Magazine()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->Eject_Magazine();
}

void UCGunComponent::Spawn_Magazine()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->Spawn_Magazine();
}

void UCGunComponent::Load_Magazine()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->Load_Magazine();
}

void UCGunComponent::End_Reload()
{
	CheckNull(GetCurrWeapon());

	GetCurrWeapon()->End_Reload();
}

void UCGunComponent::On_Begin_Aim(ACWeapon * InThisWeapon)
{
	for (ACWeapon* weapon : Weapons)
	{
		if (weapon == InThisWeapon)
			continue;

		weapon->SetHidden(true);
	}
}

void UCGunComponent::On_Begin_End()
{
	for (ACWeapon* weapon : Weapons)
		weapon->SetHidden(false);
}
