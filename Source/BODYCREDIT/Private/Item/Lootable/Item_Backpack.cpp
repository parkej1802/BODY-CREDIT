// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Lootable/Item_Backpack.h"
#include "Item/Functions/Function_Backpack.h"
#include "AC_LootingInventoryComponent.h"
#include "Inventory/AC_InventoryEquipmentComponent.h"

AItem_Backpack::AItem_Backpack()
{
	LootInventoryComp = CreateDefaultSubobject<UAC_LootingInventoryComponent>(TEXT("EquipInventoryComp"));
	ItemName = FName("Backpack");
}

void AItem_Backpack::BeginPlay()
{
	Super::BeginPlay();

	// ItemStrategy = NewObject<UFunction_Backpack>(this);

	/*if (!ItemObject) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ItemObject is nullptr in BeginPlay!"));
		return;
	}*/
	/*if (ItemObject && !ItemObject->ItemObjectInventoryComp)
	{
		ItemObject->ItemObjectInventoryComp = LootInventoryComp;

	}*/
	//ItemObject->ItemObjectInventoryComp = LootInventoryComp;
	ItemObject->ItemActorOwner = this;
	ItemObject->ItemActorOwner->LootInventoryComp = LootInventoryComp;
	/*if (LootInventoryComp)
	{
		FString Msg = FString::Printf(TEXT("LootInventoryComp is valid: %s"), *LootInventoryComp->GetName());
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, Msg);

		if (LootInventoryComp->GetOwner())
		{
			FString OwnerMsg = FString::Printf(TEXT("LootInventoryComp owner: %s"), *LootInventoryComp->GetOwner()->GetName());
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, OwnerMsg);
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LootInventoryComp is nullptr"));
	}

	if (ItemObject)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("ItemObject is valid"));

		if (ItemObject->ItemObjectInventoryComp)
		{
			FString InvMsg = FString::Printf(TEXT("ItemObject->InventoryComp is valid: %s"), *ItemObject->ItemObjectInventoryComp->GetName());
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, InvMsg);
		}
		else
		{
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ItemObject->InventoryComp is nullptr"));
		}
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ItemObject is nullptr"));
	}*/
}

