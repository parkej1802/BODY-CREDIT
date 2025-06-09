// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory_Widget.h"
#include "Inventory/Inventory_GridWidget.h"
#include "Characters/CNox_Runner.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Item/ItemObject.h"
#include "GameState_BodyCredit.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Inventory/Inventory_EquipmentWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Inventory/AC_EquipComponent.h"
#include "Components/Border.h"
#include "Inventory/AC_InventoryEquipmentComponent.h"
#include "Item/Lootable/Item_Backpack.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Characters/CNox_Controller.h"
#include "Components/CNoxHPComponent.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "AC_LootingInventoryComponent.h"

void UInventory_Widget::NativeConstruct()
{
    Super::NativeConstruct();

    PC = Cast<ACNox_Controller>(GetOwningPlayer());

    APawn* Pawn = PC->GetPawn();

    PlayerCharacter = Cast<ACNox_Runner>(Pawn);

    PlayerCharacter->SceneCapture2D->ShowOnlyActorComponents(PlayerCharacter);

    InventoryComp = PlayerCharacter->InventoryComp;

    EquipComp = PlayerCharacter->EquipComp;

	/*InventoryGridWidget->InitInventory(InventoryComp, InventoryComp->InventoryTileSize);
	InventoryGridWidget->GridID = 0;
	InventoryGridWidget->PlayerController = PC;*/
    InventoryGridWidget->SetVisibility(ESlateVisibility::Hidden);

    SetItemInventory();

    Equip_Head->PlayerCharacter = PlayerCharacter;
    Equip_Head->InitEquipment();

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

    Equip_ChestRigs->PlayerCharacter = PlayerCharacter;
    Equip_ChestRigs->InitEquipment();

    Equip_Backpack->PlayerCharacter = PlayerCharacter;
    Equip_Backpack->InitEquipment();

    if (bIsLootable) {
        LootingInventoryComp = PlayerCharacter->LootableInventoryComp;
        InventoryItemGridWidget->SetVisibility(ESlateVisibility::Visible);
        InventoryItemGridWidget->InitInventory(LootingInventoryComp, InventoryComp->InventoryTileSize);
        InventoryItemGridWidget->GridID = 1;
        InventoryItemGridWidget->PlayerController = PC;
    }
    else {

        InventoryItemGridWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (Button_Back)
    {
        Button_Back->OnClicked.AddDynamic(this, &ThisClass::OnBackClicked);
        Button_Back->OnHovered.AddDynamic(this, &ThisClass::OnBackHovered);
        Button_Back->OnUnhovered.AddDynamic(this, &ThisClass::OnBackUnhovered);
    }

    EquipComp->EquipmentChanged.AddDynamic(this, &UInventory_Widget::SetItemInventory);
}

void UInventory_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}


bool UInventory_Widget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
    return true;
}

void UInventory_Widget::OnBackClicked()
{
   InventoryComp->ShowLootableInventory();
}

void UInventory_Widget::OnBackHovered()
{
    if (Image_Button_Back_Hovered)
    {
        Image_Button_Back_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void UInventory_Widget::OnBackUnhovered()
{
    if (Image_Button_Back_Hovered)
    {
        Image_Button_Back_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

//void UInventory_Widget::IsMouseOnGrid()
//{
//    FVector2D MouseScreen;
//
//    MouseScreen = FSlateApplication::Get().GetCursorPos() -
//        FVector2D(GEngine->GameViewport->GetWindow()->GetPositionInScreen());
//
//
//    UInventory_GridWidget* NewHoveredGrid = nullptr;
//
//    if (InventoryGridWidget)
//    {
//        FGeometry GridGeo = InventoryGridWidget->GetGridContentGeometry();
//        FVector2D Local = GridGeo.AbsoluteToLocal(MouseScreen);
//
//        if (Local.X >= 0 && Local.Y >= 0 &&
//            Local.X < GridGeo.GetLocalSize().X &&
//            Local.Y < GridGeo.GetLocalSize().Y)
//        {
//            NewHoveredGrid = InventoryGridWidget;
//        }
//    }
//
//    if (InventoryItemGridWidget)
//    {
//        FGeometry GridGeo = InventoryItemGridWidget->GetGridContentGeometry();
//        FVector2D Local = GridGeo.AbsoluteToLocal(MouseScreen);
//
//        if (Local.X > 0 && Local.Y >= 0 &&
//            Local.X < GridGeo.GetLocalSize().X &&
//            Local.Y < GridGeo.GetLocalSize().Y)
//        {
//            NewHoveredGrid = InventoryItemGridWidget;
//        }
//    }
//
//    if (InventoryEquipGridWidget)
//    {
//        FGeometry GridGeo = InventoryEquipGridWidget->GetGridContentGeometry();
//        FVector2D Local = GridGeo.AbsoluteToLocal(MouseScreen);
//
//        if (Local.X > 0 && Local.Y >= 0 &&
//            Local.X < GridGeo.GetLocalSize().X &&
//            Local.Y < GridGeo.GetLocalSize().Y)
//        {
//            NewHoveredGrid = InventoryEquipGridWidget;
//        }
//    }
//
//    if (InventoryEquipChestRigsGridWidget)
//    {
//        FGeometry GridGeo = InventoryEquipChestRigsGridWidget->GetGridContentGeometry();
//        FVector2D Local = GridGeo.AbsoluteToLocal(MouseScreen);
//
//        if (Local.X > 0 && Local.Y >= 0 &&
//            Local.X < GridGeo.GetLocalSize().X &&
//            Local.Y < GridGeo.GetLocalSize().Y)
//        {
//            NewHoveredGrid = InventoryEquipChestRigsGridWidget;
//        }
//    }
//
//    if (CurrentHoveredGrid != NewHoveredGrid)
//    {
//        CurrentHoveredGrid = NewHoveredGrid;
//        if (CurrentHoveredGrid)
//        {
//            UDragDropOperation* CurrentOp = UWidgetBlueprintLibrary::GetDragDroppingContent();
//            if (!CurrentOp) return;
//
//            UItemObject* ItemObject = Cast<UItemObject>(CurrentOp->Payload);
//            if (IsValid(ItemObject))
//            {
//                ItemObject->StartPosition.X = 0;
//                ItemObject->StartPosition.Y = 0;
//            }
//
//			/*GEngine->AddOnScreenDebugMessage(
//				 -1, 1.f, FColor::Green,
//				 FString::Printf(TEXT("Hovered Grid ID: %d"), CurrentHoveredGrid->GridID));*/
//        }
//        else
//        {
//			/*GEngine->AddOnScreenDebugMessage(
//				 -1, 1.f, FColor::Red, TEXT("No Grid Hovered"));*/
//        }
//   }
//}

FReply UInventory_Widget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    /*FEventReply reply;

    reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    if (InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton")))
    {
        reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
    }

    return reply.NativeReply;*/
    return FReply::Handled();
}

bool UInventory_Widget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
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

    AGameState_BodyCredit* MyGameState = GetWorld()->GetGameState<AGameState_BodyCredit>();

    MyGameState->SpawnItemFromActor(ItemObject, InventoryComp->GetOwner(), true);

    return true;
}

void UInventory_Widget::SetItemInventory()
{
    if (UItemObject** FoundChestRigs = EquipComp->EquippedItems.Find(EPlayerPart::ChestRigs)) {
        if (FoundChestRigs && InventoryEquipGridWidget) {
            ChestRigsItem = *FoundChestRigs;

            if (ChestRigsItem) {
                if (ChestRigsItem->ItemActorOwner->LootInventoryComp) {
                    if (EquipChestRigsInventoryComp != ChestRigsItem->ItemActorOwner->LootInventoryComp) {
                        EquipChestRigsInventoryComp = ChestRigsItem->ItemActorOwner->LootInventoryComp;

                        InventoryEquipChestRigsGridWidget->SetVisibility(ESlateVisibility::Visible);
                        InventoryEquipChestRigsGridWidget->InitInventory(EquipChestRigsInventoryComp, InventoryComp->InventoryTileSize);
                        InventoryEquipChestRigsGridWidget->GridID = 3;
                        InventoryEquipChestRigsGridWidget->PlayerController = PC;
                    }
                    else {
                        InventoryEquipChestRigsGridWidget->SetVisibility(ESlateVisibility::Visible);
                    }
                }
            }
        }
    }
    else {
        ChestRigsItem = nullptr;
        EquipChestRigsInventoryComp = nullptr;
        InventoryEquipChestRigsGridWidget->SetVisibility(ESlateVisibility::Hidden);
        InventoryEquipChestRigsGridWidget->ClearInventory();
    }

	if (UItemObject** FoundBackpack = EquipComp->EquippedItems.Find(EPlayerPart::Backpack)) {

		if (FoundBackpack && InventoryEquipGridWidget) {
			BackpackItem = *FoundBackpack;

			if (BackpackItem) {

				if (BackpackItem->ItemActorOwner->LootInventoryComp) {

					if (EquipBackpackInventoryComp != BackpackItem->ItemActorOwner->LootInventoryComp) {
						EquipBackpackInventoryComp = BackpackItem->ItemActorOwner->LootInventoryComp;

						InventoryEquipGridWidget->SetVisibility(ESlateVisibility::Visible);
						InventoryEquipGridWidget->InitInventory(EquipBackpackInventoryComp, InventoryComp->InventoryTileSize);
						InventoryEquipGridWidget->GridID = 2;
						InventoryEquipGridWidget->PlayerController = PC;
					}
					else {

						InventoryEquipGridWidget->SetVisibility(ESlateVisibility::Visible);
					}
				}
			}
		}
	}
	else {
		BackpackItem = nullptr;
		EquipBackpackInventoryComp = nullptr;
		InventoryEquipGridWidget->SetVisibility(ESlateVisibility::Hidden);
		InventoryEquipGridWidget->ClearInventory();
	}
}


void UInventory_Widget::PlayerStatChange()
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
