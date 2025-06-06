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

void UFunction_Backpack::Use(UItemObject* ItemObject)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("USE Working"));

	if (!ItemObject) return;

	UWorld* World = ItemObject->GetWorld();
	if (!World) return;

	PC = Cast<ACNox_Controller>(World->GetFirstPlayerController());
	if (!PC) return;

	APawn* Pawn = PC->GetPawn();

	PlayerCharacter = Cast<ACNox_Runner>(Pawn);

	if (!InventoryWidget)
	{
		InventoryWidget = LoadClass<UInventory_ItemStrategy>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Inventory/WBP_ItemStrategy.WBP_ItemStrategy'"));
	}

	if (InventoryWidget)
	{
		InventoryUI = CreateWidget<UInventory_ItemStrategy>(GetWorld(), InventoryWidget);
		InventoryUI->InventoryComp = Cast<UAC_InventoryBaseComponent>(ItemObject->ItemActorOwner->LootInventoryComp);
		InventoryUI->ItemInventoryGridWidget->InitInventory(InventoryUI->InventoryComp, PlayerCharacter->InventoryComp->InventoryTileSize);
		InventoryUI->ItemInventoryGridWidget->PlayerController = PC;
	}

	if (InventoryUI)
	{
		InventoryUI->AddToViewport();
	}
}