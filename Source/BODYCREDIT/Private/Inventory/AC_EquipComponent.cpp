﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/AC_EquipComponent.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Inventory/Inventory_Widget.h"
#include "Misc/EnumRange.h"
#include "Session/NetGameInstance.h"
#include "Components/CNoxHPComponent.h"
#include "Components/Image.h"
#include "Components/Border.h"

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
			PlayerCharacter->GetHead()->SetSkeletalMeshAsset(Item->ItemData.SkeletalMesh);
			/*InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_Head->GetContent();

			if (UImage* InnerImage = Cast<UImage>(InnerWidget))
			{
				InnerImage->SetVisibility(ESlateVisibility::Hidden);
			}*/
			break;
		case EPlayerPart::Body:
			PlayerCharacter->GetUpperBody()->SetSkeletalMeshAsset(Item->ItemData.SkeletalMesh);
			/*InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_Body->GetContent();

			if (UImage* InnerImage = Cast<UImage>(InnerWidget))
			{
				InnerImage->SetVisibility(ESlateVisibility::Hidden);
			}*/
			break;
		case EPlayerPart::Arm:
			PlayerCharacter->GetArms()->SetSkeletalMeshAsset(Item->ItemData.SkeletalMesh);
			/*InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_Arm->GetContent();

			if (UImage* InnerImage = Cast<UImage>(InnerWidget))
			{
				InnerImage->SetVisibility(ESlateVisibility::Hidden);
			}*/
			break;
		case EPlayerPart::Leg:
			PlayerCharacter->GetLowerBody()->SetSkeletalMeshAsset(Item->ItemData.SkeletalMesh);
			/*InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_Leg->GetContent();

			if (UImage* InnerImage = Cast<UImage>(InnerWidget))
			{
				InnerImage->SetVisibility(ESlateVisibility::Hidden);
			}*/
			break;
		/*case EPlayerPart::Weapon1:
			InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_Weapon_1->GetContent();

			if (UImage* InnerImage = Cast<UImage>(InnerWidget))
			{
				InnerImage->SetVisibility(ESlateVisibility::Hidden);
			}
			break;
		case EPlayerPart::Weapon2:
			InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_Weapon_2->GetContent();

			if (UImage* InnerImage = Cast<UImage>(InnerWidget))
			{
				InnerImage->SetVisibility(ESlateVisibility::Hidden);
			}
			break;
		case EPlayerPart::Backpack:
			InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_Backpack->GetContent();

			if (UImage* InnerImage = Cast<UImage>(InnerWidget))
			{
				InnerImage->SetVisibility(ESlateVisibility::Hidden);
			}
			break;
		case EPlayerPart::ChestRigs:
			InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_ChestRigs->GetContent();

			if (UImage* InnerImage = Cast<UImage>(InnerWidget))
			{
				InnerImage->SetVisibility(ESlateVisibility::Hidden);
			}
			break;*/
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

		/*switch (Part)
		{
		case EPlayerPart::Weapon1:
			InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_Weapon_1->GetContent();

			if (UImage* InnerImage = Cast<UImage>(InnerWidget))
			{
				InnerImage->SetVisibility(ESlateVisibility::Visible);
			}
			break;
		case EPlayerPart::Weapon2:
			InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_Weapon_2->GetContent();

			if (UImage* InnerImage = Cast<UImage>(InnerWidget))
			{
				InnerImage->SetVisibility(ESlateVisibility::Visible);
			}
			break;
		case EPlayerPart::Backpack:
			InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_Backpack->GetContent();

			if (UImage* InnerImage = Cast<UImage>(InnerWidget))
			{
				InnerImage->SetVisibility(ESlateVisibility::Visible);
			}
			break;
		case EPlayerPart::ChestRigs:
			InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_ChestRigs->GetContent();

			if (UImage* InnerImage = Cast<UImage>(InnerWidget))
			{
				InnerImage->SetVisibility(ESlateVisibility::Visible);
			}
			break;
		default:
			break;
		}*/

		if (PlayerCharacter && PlayerCharacter->DefaultMeshes.Contains(Part))
		{
			USkeletalMesh* DefaultMesh = PlayerCharacter->DefaultMeshes[Part];
			switch (Part)
			{
			case EPlayerPart::Head:
				PlayerCharacter->GetHead()->SetSkeletalMeshAsset(DefaultMesh);
				/*InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_Head->GetContent();

				if (UImage* InnerImage = Cast<UImage>(InnerWidget))
				{
					InnerImage->SetVisibility(ESlateVisibility::Visible);
				}*/
				break;
			case EPlayerPart::Body:
				PlayerCharacter->GetUpperBody()->SetSkeletalMeshAsset(DefaultMesh);
				/*InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_Body->GetContent();

				if (UImage* InnerImage = Cast<UImage>(InnerWidget))
				{
					InnerImage->SetVisibility(ESlateVisibility::Visible);
				}*/
				break;
			case EPlayerPart::Arm:
				PlayerCharacter->GetArms()->SetSkeletalMeshAsset(DefaultMesh);
				/*InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_Arm->GetContent();

				if (UImage* InnerImage = Cast<UImage>(InnerWidget))
				{
					InnerImage->SetVisibility(ESlateVisibility::Visible);
				}*/
				break;
			case EPlayerPart::Leg:
				PlayerCharacter->GetLowerBody()->SetSkeletalMeshAsset(DefaultMesh);
				/*InnerWidget = PlayerCharacter->InventoryComp->InventoryMainUI->Border_Leg->GetContent();

				if (UImage* InnerImage = Cast<UImage>(InnerWidget))
				{
					InnerImage->SetVisibility(ESlateVisibility::Visible);
				}*/
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
	PlayerCharacter->HPComp->Health += Direction * Item->ItemData.StatIncrease.Health;
	PlayerCharacter->HPComp->Strength += Direction * Item->ItemData.StatIncrease.Strength;
	PlayerCharacter->HPComp->MovementSpeed += Direction * Item->ItemData.StatIncrease.MoveSpeed;
	PlayerCharacter->HPComp->Defense += Direction * Item->ItemData.StatIncrease.Armor;
	PlayerCharacter->HPComp->Stamina += Direction * Item->ItemData.StatIncrease.Stamina;
	PlayerCharacter->HPComp->Weight += Direction * Item->ItemData.Weight;
	PlayerCharacter->HPComp->Humanity += Direction * Item->ItemData.StatIncrease.Humanity;
}

int32 UAC_EquipComponent::CalculatePriceOfEquippedItem()
{
	int32 TotalPrice = 0;

	for (const auto& Pair : EquippedItems)
	{
		if (const UItemObject* Item = Pair.Value)
		{
			TotalPrice += Item->ItemData.Price;
		}
	}

	return TotalPrice;
}

