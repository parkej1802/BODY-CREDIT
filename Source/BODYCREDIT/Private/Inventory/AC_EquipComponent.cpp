// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/AC_EquipComponent.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Inventory/Inventory_Widget.h"
#include "Misc/EnumRange.h"

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
	if (UItemObject** FoundBackpack = EquippedItems.Find(EPlayerPart::Backpack))
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
	}
	
}

void UAC_EquipComponent::EquipItem(EPlayerPart Part, UItemObject* Item)
{
	if (!Item) return;

	EquippedItems.Add(Part, Item);

	FString Msg = FString::Printf(TEXT("[EquipItem] Part: %d, ItemObject ptr: %p, InventoryComp ptr: %p"),
		(int)Part,
		Item,
		Item->ItemObjectInventoryComp);
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, Msg);
}

void UAC_EquipComponent::UnequipItem(EPlayerPart Part)
{
	EquippedItems.Remove(Part);
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
