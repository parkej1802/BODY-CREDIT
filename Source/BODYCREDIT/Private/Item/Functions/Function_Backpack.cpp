// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Functions/Function_Backpack.h"
#include "Item/Lootable/Item_Backpack.h"
#include "Inventory/Inventory_ItemStrategy.h"
#include "Inventory/Inventory_ItemWidget.h"
#include "Inventory/AC_InventoryBaseComponent.h"
#include "Characters/CNox_Controller.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Inventory/Inventory_GridWidget.h"
#include "Item/Item_Base.h"
#include "Inventory/Inventory_ItemInventoryWidget.h"

void UFunction_Backpack::Use(UItemObject* ItemObject)
{
	if (ItemObject->bIsUseFunction) {
		InventoryUI->OnExitClicked();
		return;
	}

	AItem_Base* Owner = ItemObject->ItemActorOwner.Get();

	UWorld* World = Owner->GetWorld();
	
	PC = Cast<ACNox_Controller>(World->GetFirstPlayerController());

	APawn* Pawn = PC->GetPawn();

	PlayerCharacter = Cast<ACNox_Runner>(Pawn);
	ItemObject->bIsUseFunction = !ItemObject->bIsUseFunction;

	if (!InventoryWidget)
	{
		//InventoryWidget = LoadClass<UInventory_ItemInventoryWidget>(nullptr, TEXT("/Game/Inventory/WBP_ItemInventoryWidget.WBP_ItemInventoryWidget_C"));
		InventoryWidget = LoadClass<UInventory_ItemStrategy>(nullptr, TEXT("/Game/Inventory/WBP_ItemStrategy.WBP_ItemStrategy_C"));
	}

	if (InventoryWidget) 
	{
		InventoryUI = CreateWidget<UInventory_ItemStrategy>(World, InventoryWidget);
		InventoryUI->InventoryComp = Cast<UAC_InventoryBaseComponent>(ItemObject->ItemActorOwner->LootInventoryComp);

		InventoryUI->ItemInventoryGridWidget->InitInventory(InventoryUI->InventoryComp, PlayerCharacter->InventoryComp->InventoryTileSize);
		InventoryUI->ItemInventoryGridWidget->PlayerController = PC;

		InventoryUI->ItemObject = ItemObject;


	/*	ItemInventoryUI = CreateWidget<UInventory_ItemInventoryWidget>(World, InventoryWidget);
		if (!ItemInventoryUI)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("ItemInventoryUI Create Failed"));
			return;
		}
		ItemInventoryUI->InventoryStrategyUI->InventoryComp = Cast<UAC_InventoryBaseComponent>(ItemObject->ItemActorOwner->LootInventoryComp);

		ItemInventoryUI->InventoryStrategyUI->ItemInventoryGridWidget->InitInventory(ItemInventoryUI->InventoryStrategyUI->InventoryComp, PlayerCharacter->InventoryComp->InventoryTileSize);
		ItemInventoryUI->InventoryStrategyUI->ItemInventoryGridWidget->PlayerController = PC;
		ItemInventoryUI->InventoryStrategyUI->ParentWidget = ItemInventoryUI;*/
	}

	if (InventoryUI)
	{
		InventoryUI->AddToViewport();
	}

	/*if (ItemInventoryUI)
	{
		ItemInventoryUI->AddToViewport();
	}*/
}
