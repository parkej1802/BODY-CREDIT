﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_WorkShop.h"
#include "Components/Button.h"
#include "Lobby/LobbyWidget_Selection.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Inventory/Inventory_GridWidget.h"
#include "Components/Border.h"
#include "Inventory/Inventory_EquipmentWidget.h"
#include "Inventory/Inventory_Widget.h"
#include "Inventory/AC_EquipComponent.h"
#include "Item/ItemObject.h"
#include "Item/Item_Base.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Session/NetGameInstance.h"

void ULobbyWidget_WorkShop::NativeConstruct()
{
    Super::NativeConstruct();

    PC = GetOwningPlayer();
    FInputModeUIOnly InputMode;
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = true;

    if (Button_Back)
    {
        Button_Back->OnClicked.AddDynamic(this, &ULobbyWidget_WorkShop::OnBackClicked);
    }

    APawn* Pawn = PC->GetPawn();

    PlayerCharacter = Cast<ACNox_Runner>(Pawn);
    InventoryComp = PlayerCharacter->InventoryComp;
    EquipComp = PlayerCharacter->EquipComp;

    InventoryGridWidget->InitInventory(InventoryComp, InventoryComp->InventoryTileSize);
    InventoryGridWidget->GridID = 4;
    InventoryGridWidget->PlayerController = PC;

    if (Equip_Head)
    {
        Equip_Head->PlayerCharacter = PlayerCharacter;
        Equip_Head->InitEquipment();
    }

    Equip_Arm->PlayerCharacter = PlayerCharacter;
    Equip_Arm->InitEquipment();

    Equip_Body->PlayerCharacter = PlayerCharacter;
    Equip_Body->InitEquipment();

    Equip_Leg->PlayerCharacter = PlayerCharacter;
    Equip_Leg->InitEquipment();

    Equip_Weapon_1->PlayerCharacter = PlayerCharacter;
    Equip_Weapon_1->InitEquipment();

    Equip_Weapon_2->PlayerCharacter = PlayerCharacter;
    Equip_Weapon_2->InitEquipment();

    if (Equip_ChestRigs_1) {
        Equip_ChestRigs_1->PlayerCharacter = PlayerCharacter;
        Equip_ChestRigs_1->InitEquipment();
    }

    Equip_Backpack_1->PlayerCharacter = PlayerCharacter;
    Equip_Backpack_1->InitEquipment();

    SetItemInventory();

    EquipComp->EquipmentChanged.AddDynamic(this, &ULobbyWidget_WorkShop::SetItemInventory);
}

void ULobbyWidget_WorkShop::OnBackClicked()
{
    if (LobbySelectionWidgetClass)
    {
        LobbyWidget_Selection = CreateWidget<ULobbyWidget_Selection>(GetWorld(), LobbySelectionWidgetClass);
        if (LobbyWidget_Selection)
        {
            LobbyWidget_Selection->AddToViewport();

            this->RemoveFromParent();
        }
    }

    UNetGameInstance* GI = Cast<UNetGameInstance>(GetGameInstance());
	if (GI) {
        GI->SavedEquippedItems.Empty();
		for (auto& Pair : EquipComp->EquippedItems)
		{
			if (Pair.Value)
			{
				GI->SavedEquippedItems.Add(Pair.Key, Pair.Value->ExportData());
			}
		}

		/*for (UItemObject* Item : InventoryComp->Items)
		{
			if (Item)
			{
				GI->SavedInventoryItems.Add(Item->ExportData());
			}
		}*/
	}
}

void ULobbyWidget_WorkShop::SetItemInventory()
{
    if (UItemObject** FoundChestRigs = EquipComp->EquippedItems.Find(EPlayerPart::ChestRigs)) {
        if (FoundChestRigs && InventoryEquipChestRigsGridWidget_1) {
            ChestRigsItem = *FoundChestRigs;

            if (ChestRigsItem) {
                if (ChestRigsItem->ItemActorOwner->LootInventoryComp) {
                    if (EquipChestRigsInventoryComp != ChestRigsItem->ItemActorOwner->LootInventoryComp) {
                        EquipChestRigsInventoryComp = ChestRigsItem->ItemActorOwner->LootInventoryComp;

                        InventoryEquipChestRigsGridWidget_1->SetVisibility(ESlateVisibility::Visible);
                        InventoryEquipChestRigsGridWidget_1->InitInventory(EquipChestRigsInventoryComp, InventoryComp->InventoryTileSize);
                        InventoryEquipChestRigsGridWidget_1->GridID = 3;
                        InventoryEquipChestRigsGridWidget_1->PlayerController = PC;
                    }
                    else {
                        InventoryEquipChestRigsGridWidget_1->SetVisibility(ESlateVisibility::Visible);
                    }
                }
            }
        }
    }
    else {
        ChestRigsItem = nullptr;
        EquipChestRigsInventoryComp = nullptr;
        InventoryEquipChestRigsGridWidget_1->SetVisibility(ESlateVisibility::Hidden);
        InventoryEquipChestRigsGridWidget_1->ClearInventory();
    }

    if (UItemObject** FoundBackpack = EquipComp->EquippedItems.Find(EPlayerPart::Backpack)) {

        if (FoundBackpack && InventoryEquipBackpackGridWidget_1) {
            BackpackItem = *FoundBackpack;

            if (BackpackItem) {
                if (BackpackItem->ItemActorOwner->LootInventoryComp) {

                    if (EquipBackpackInventoryComp != BackpackItem->ItemActorOwner->LootInventoryComp) {
                        EquipBackpackInventoryComp = BackpackItem->ItemActorOwner->LootInventoryComp;

                        InventoryEquipBackpackGridWidget_1->SetVisibility(ESlateVisibility::Visible);
                        InventoryEquipBackpackGridWidget_1->InitInventory(EquipBackpackInventoryComp, InventoryComp->InventoryTileSize);
                        InventoryEquipBackpackGridWidget_1->GridID = 2;
                        InventoryEquipBackpackGridWidget_1->PlayerController = PC;
                    }
                    else {

                        InventoryEquipBackpackGridWidget_1->SetVisibility(ESlateVisibility::Visible);
                    }
                }
            }
        }
    }
    else {
        BackpackItem = nullptr;
        EquipBackpackInventoryComp = nullptr;
        InventoryEquipBackpackGridWidget_1->SetVisibility(ESlateVisibility::Hidden);
        InventoryEquipBackpackGridWidget_1->ClearInventory();
    }
}

FReply ULobbyWidget_WorkShop::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    return FReply::Handled();
}

bool ULobbyWidget_WorkShop::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
    return true;
}

bool ULobbyWidget_WorkShop::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
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

    return true;
}

void ULobbyWidget_WorkShop::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

