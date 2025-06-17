// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/AC_EquipComponent.h"
#include "Global.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Inventory/Inventory_Widget.h"
#include "Misc/EnumRange.h"
#include "Session/NetGameInstance.h"
#include "Components/CNoxHPComponent.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/CWeaponComponent.h"
#include "Item/ItemObject.h"
#include "Item/Item_Base.h"
#include "AC_LootingInventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Equipments/Weapons/CWeapon_Attachment.h"
#include "Items/Equipments/Weapons/CWeapon_Data.h"

// Sets default values for this component's properties
UAC_EquipComponent::UAC_EquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	

}


// Called when the game starts
void UAC_EquipComponent::BeginPlay()
{
	Super::BeginPlay();
	 
	AActor* OwnerActor = GetOwner();

	if (OwnerActor)
	{
		PlayerCharacter = Cast<ACNox_Runner>(OwnerActor);
	}
}


// Called every frame
void UAC_EquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsChanged) {
		IsChanged = false;
		OnEquipmentChanged();
	}
	/*if (UItemObject** FoundBackpack = EquippedItems.Find(EPlayerPart::Backpack))
	{
		BackpackItem = *FoundBackpack;
		if (BackpackItem) {
			if (BackpackItem->ItemObjectInventoryComp)
			{
				FString DebugMsg = FString::Printf(TEXT("BackpackItem->InventoryComp Address: %p"), BackpackItem->ItemObjectInventoryComp);
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, DebugMsg);
				}
			}
			else {
				FString DebugMsg = FString::Printf(TEXT("BackpackItem->InventoryComp Address: %p"), BackpackItem->ItemObjectInventoryComp);
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Yellow, DebugMsg);
				}
			}
		}
	}*/
	
}

void UAC_EquipComponent::EquipItem(EPlayerPart Part, UItemObject* Item)
{
	if (!Item) return;

	SetPlayerStat(Item, 1);
	EquippedItems.Add(Part, Item);

	if (Item->ItemData.SkeletalMesh)
	{		
		switch (Part)
		{
		case EPlayerPart::Head:
			PlayerCharacter->GetHair()->SetSkeletalMeshAsset(Item->ItemData.SkeletalMesh);

			break;
		case EPlayerPart::Body:
			PlayerCharacter->GetUpperBody()->SetSkeletalMeshAsset(Item->ItemData.SkeletalMesh);

			break;
		case EPlayerPart::Arm:
			PlayerCharacter->GetArms()->SetSkeletalMeshAsset(Item->ItemData.SkeletalMesh);

			break;
		case EPlayerPart::Leg:
			PlayerCharacter->GetLowerBody()->SetSkeletalMeshAsset(Item->ItemData.SkeletalMesh);

			break;
		case EPlayerPart::Weapon1:
			{
				PlayerCharacter->GetWeapon1()->SetSkeletalMeshAsset(Item->ItemData.SkeletalMesh);
			}
			break;
		case EPlayerPart::Weapon2:
			PlayerCharacter->GetWeapon2()->SetSkeletalMeshAsset(Item->ItemData.SkeletalMesh);
			break;
		case EPlayerPart::Backpack:
			PlayerCharacter->GetBackpack()->SetSkeletalMeshAsset(Item->ItemData.SkeletalMesh);
			break;
		case EPlayerPart::ChestRigs:
			PlayerCharacter->GetChestRig()->SetSkeletalMeshAsset(Item->ItemData.SkeletalMesh);
			break;
		default:
			break;
		}
	}
}

void UAC_EquipComponent::UnequipItem(EPlayerPart Part)
{
	if (EquippedItems.Contains(Part))
	{
		SetPlayerStat(EquippedItems[Part], -1);
		EquippedItems.Remove(Part);
		
		if (PlayerCharacter && PlayerCharacter->DefaultMeshes.Contains(Part))
		{			
			USkeletalMesh* DefaultMesh = PlayerCharacter->DefaultMeshes[Part];
			switch (Part)
			{
				case EPlayerPart::Head:
					PlayerCharacter->GetHair()->SetSkeletalMeshAsset(DefaultMesh);
					break;
				case EPlayerPart::Body:
					PlayerCharacter->GetUpperBody()->SetSkeletalMeshAsset(DefaultMesh);
					break;
				case EPlayerPart::Arm:
					PlayerCharacter->GetArms()->SetSkeletalMeshAsset(DefaultMesh);
					break;
				case EPlayerPart::Leg:
					PlayerCharacter->GetLowerBody()->SetSkeletalMeshAsset(DefaultMesh);
					break;
				case EPlayerPart::Weapon1:
					PlayerCharacter->GetWeapon1()->SetSkeletalMeshAsset(DefaultMesh);
					if (UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(PlayerCharacter))
						weapon->SetUnarmedMode();
					break;
				case EPlayerPart::Weapon2:
					PlayerCharacter->GetWeapon2()->SetSkeletalMeshAsset(DefaultMesh);
					if (UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(PlayerCharacter))
						weapon->SetUnarmedMode();
					break;
				case EPlayerPart::Backpack:
					PlayerCharacter->GetBackpack()->SetSkeletalMeshAsset(DefaultMesh);
					break;
				case EPlayerPart::ChestRigs:
					PlayerCharacter->GetChestRig()->SetSkeletalMeshAsset(DefaultMesh);
					break;
				default:
					break;
			}
		}
	}
}

bool UAC_EquipComponent::IsEquip(EPlayerPart Part)
{
	if (UItemObject** FoundItem = EquippedItems.Find(Part))
	{
		return true;
	}

	return false;
}

void UAC_EquipComponent::SetHasBackpack()
{
	if (EquippedItems.Contains(EPlayerPart::Backpack)) {
		PlayerCharacter->InventoryComp->InventoryMainUI->bHasBackpack = true;
	}
	else {
		PlayerCharacter->InventoryComp->InventoryMainUI->bHasBackpack = false;
	}
}

void UAC_EquipComponent::OnEquipmentChanged()
{
	EquipmentChanged.Broadcast();
}

UItemObject* UAC_EquipComponent::CreateItemFromData(const FItemSaveData& Data)
{
	UItemObject* NewItem = NewObject<UItemObject>(this);
	if (NewItem)
	{
		NewItem->ImportData(Data);
	}
	return NewItem;
}

void UAC_EquipComponent::SetPlayerStat(UItemObject* Item, int32 Direction)
{
	PlayerCharacter->HPComp->MaxHealth += Direction * Item->ItemData.StatIncrease.Health;
	PlayerCharacter->HPComp->MaxStamina += Direction * Item->ItemData.StatIncrease.Stamina;
	PlayerCharacter->HPComp->Strength += Direction * Item->ItemData.StatIncrease.Strength;
	PlayerCharacter->HPComp->Defense += Direction * Item->ItemData.StatIncrease.Armor;
	PlayerCharacter->HPComp->Weight += Direction * Item->ItemData.Weight;
	PlayerCharacter->HPComp->MovementSpeed += Direction * Item->ItemData.StatIncrease.MoveSpeed - (PlayerCharacter->HPComp->Weight - 60);
	PlayerCharacter->HPComp->Humanity += Direction * Item->ItemData.StatIncrease.Humanity;

	PlayerCharacter->HPComp->InitStatus();
	// PlayerCharacter->Movement->InitMovement(PlayerCharacter->HPComp->MovementSpeed);
	// PlayerCharacter->Weapon->Init(); 
}

int32 UAC_EquipComponent::CalculatePriceOfEquippedItem()
{
	int32 TotalPrice = 0;

	for (auto& Pair : EquippedItems)
	{
		if (UItemObject* Item = Pair.Value)
		{
			TotalPrice += Item->GetSellPrice();

			if (Item->ItemData.ItemType == EPlayerPart::Backpack || Item->ItemData.ItemType == EPlayerPart::ChestRigs)
			{
				
				TotalPrice += CalculatePriceRecursively(Item);
			}
		}
	}

	return TotalPrice;
}

int32 UAC_EquipComponent::CalculatePriceRecursively(UItemObject* ItemObject)
{
	AItem_Base* ItemActor = ItemObject->ItemActorOwner.Get();

	if (!ItemObject || !ItemActor || !ItemActor->LootInventoryComp)
	{
		return 0;
	}

	int32 ContainerPrice = 0;

	TMap<UItemObject*, FInventoryTile> ContainedItems = ItemObject->ItemActorOwner->LootInventoryComp->GetAllItems();

	for (const TPair<UItemObject*, FInventoryTile>& Pair : ContainedItems)
	{
		UItemObject* ContainedItem = Pair.Key;
		if (!ContainedItem) continue;

		ContainerPrice += ContainedItem->GetSellPrice();

		if (ContainedItem->ItemData.ItemType == EPlayerPart::Backpack || ContainedItem->ItemData.ItemType == EPlayerPart::ChestRigs)
		{
			ContainerPrice += CalculatePriceRecursively(ContainedItem);
		}	
	}

	return ContainerPrice;
}

void UAC_EquipComponent::RefreshEquip()
{
	TArray<EPlayerPart> EquippedParts;
	EquippedItems.GetKeys(EquippedParts);

	for (EPlayerPart Part : EquippedParts)
	{
		UnequipItem(Part);
	}
}
