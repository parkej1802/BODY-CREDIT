// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/Market_ItemTile.h"
#include "Components/Button.h"
#include "GameState_BodyCredit.h"
#include "Item/Item_Base.h"
#include "Inventory/AC_InventoryBaseComponent.h"
#include "Inventory/Inventory_EquipmentTile.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Characters/CNox_Controller.h"

void UMarket_ItemTile::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_BuyItem)
	{
		Button_BuyItem->OnClicked.AddDynamic(this, &ThisClass::OnBuyItemClicked);
	}

	PC = Cast<ACNox_Controller>(GetOwningPlayer());

	APawn* Pawn = PC->GetPawn();

	PlayerCharacter = Cast<ACNox_Runner>(Pawn);

	InventoryComp = PlayerCharacter->InventoryComp;
}

void UMarket_ItemTile::OnBuyItemClicked()
{
    UItemObject* NewItemObject = NewObject<UItemObject>();
    NewItemObject->ItemData = ItemData;

    AGameState_BodyCredit* GameState = GetWorld()->GetGameState<AGameState_BodyCredit>();
    if (GameState)
    {
        GameState->SpawnItemPlayerInventory(NewItemObject, GetOwningPlayerPawn(), false);
    }
}

bool UMarket_ItemTile::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemObject* ItemObject = Cast<UItemObject>(InOperation->Payload);

	if (UInventory_ItemWidget* DraggedWidget = Cast<UInventory_ItemWidget>(InOperation->DefaultDragVisual))
	{
		DraggedWidget->IsMoving = false;
	}

	if (UInventory_EquipmentTile* InventoryItemTileUI = Cast<UInventory_EquipmentTile>(InOperation->DefaultDragVisual)) {
		InventoryItemTileUI->IsMoving = false;

	}

	InventoryComp->TryAddItem(ItemObject);

	return true;
}
