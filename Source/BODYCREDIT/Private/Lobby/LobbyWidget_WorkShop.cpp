// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_WorkShop.h"

#include "AC_LootingInventoryComponent.h"
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
#include "Components/SceneCaptureComponent2D.h"
#include "Characters/CNox_Controller.h"
#include "Components/CNoxHPComponent.h"
#include "Components/TextBlock.h"
#include "AC_LootingInventoryComponent.h"
#include "Components/Image.h"

void ULobbyWidget_WorkShop::NativeConstruct()
{
    Super::NativeConstruct();

    PC = Cast<ACNox_Controller>(GetOwningPlayer());
    FInputModeGameAndUI InputMode;
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = true;

    if (Button_Back)
    {
        Button_Back->OnClicked.AddDynamic(this, &ThisClass::OnBackClicked);
        Button_Back->OnHovered.AddDynamic(this, &ThisClass::OnBackHovered);
        Button_Back->OnUnhovered.AddDynamic(this, &ThisClass::OnBackUnhovered);
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

    PlayerStatChange();
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

void ULobbyWidget_WorkShop::OnBackHovered()
{
    if (Image_Button_Back_Hovered)
    {
        Image_Button_Back_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_WorkShop::OnBackUnhovered()
{
    if (Image_Button_Back_Hovered)
    {
        Image_Button_Back_Hovered->SetVisibility(ESlateVisibility::Hidden);
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

    InventoryComp->TryAddItem(ItemObject);

    return true;
}

void ULobbyWidget_WorkShop::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (PlayerCharacter->EquipComp->IsStatChanged) 
    {
        PlayerCharacter->EquipComp->IsStatChanged = false;
        PlayerStatChange();
    }
}

void ULobbyWidget_WorkShop::PlayerStatChange()
{
    float Power = PlayerCharacter->HPComp->Strength;
    float Health = PlayerCharacter->HPComp->Health;
    float Defense = PlayerCharacter->HPComp->Defense;
    float MovementSpeed = PlayerCharacter->HPComp->MovementSpeed;
    float Weight = PlayerCharacter->HPComp->Weight;
    float Stamina = PlayerCharacter->HPComp->Stamina;
    float Humanity = PlayerCharacter->HPComp->Humanity;

    Text_Power->SetText(FText::AsNumber(Power));
    Text_Health->SetText(FText::AsNumber(Health));
    Text_Speed->SetText(FText::AsNumber(MovementSpeed));
    Text_Weight->SetText(FText::AsNumber(Weight));
    Text_Energy->SetText(FText::AsNumber(Stamina));
    Text_Humanity->SetText(FText::AsNumber(Humanity));
}