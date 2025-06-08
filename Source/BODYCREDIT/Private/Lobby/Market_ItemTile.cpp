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
#include "Components/TextBlock.h"
#include "Session/NetGameInstance.h"
#include "Lobby/LobbyWidget_InsufficientGold.h"
#include "Lobby/LobbyWidget_Market.h"


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

	Text_Name->SetText(FText::FromName(ItemData.ItemName));
	Text_Price->SetText(FText::AsNumber(ItemData.Price));

	GI = Cast<UNetGameInstance>(GetGameInstance());

}

void UMarket_ItemTile::OnBuyItemClicked()
{
	if (ItemData.Price > GI->PlayerGold)
	{
		if (InsufficientFundWidget)
		{
			InsufficientFundUI = CreateWidget<ULobbyWidget_InsufficientGold>(this, InsufficientFundWidget);
		}
		if (InsufficientFundUI)
		{
			InsufficientFundUI->AddToViewport();
		}
		return;
	}

    UItemObject* NewItemObject = NewObject<UItemObject>();
    NewItemObject->ItemData = ItemData;

    AGameState_BodyCredit* GameState = GetWorld()->GetGameState<AGameState_BodyCredit>();
    if (GameState)
    {
        GameState->SpawnItemPlayerInventory(NewItemObject, GetOwningPlayerPawn(), false);
    }

	if (GI) {
		int32 NewGold = GI->PlayerGold - ItemData.Price;
		GI->SetPlayerGold(NewGold);
		if (OwningMarket) {
			OwningMarket->UpdatePlayerGoldText(NewGold);
		}
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