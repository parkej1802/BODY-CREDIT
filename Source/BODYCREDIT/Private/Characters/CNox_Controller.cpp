// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CNox_Controller.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "Inventory/Inventory_ItemWidget.h"
#include "Inventory/Inventory_EquipmentTile.h"
#include "Item/ItemObject.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

ACNox_Controller::ACNox_Controller()
{
	
}

void ACNox_Controller::SetupInputComponent()
{
    Super::SetupInputComponent();

    // InputComponent->BindKey(EKeys::R, IE_Pressed, this, &ACNox_Controller::RotateDraggedItem);
}

void ACNox_Controller::RotateDraggedItem()
{
    UDragDropOperation* DragOp = UWidgetBlueprintLibrary::GetDragDroppingContent();
    if (!DragOp) return;

    UItemObject* ItemObj = Cast<UItemObject>(DragOp->Payload);
    if (!IsValid(ItemObj)) return;

    ItemObj->Rotate();

    if (UInventory_ItemWidget* ItemWidget = Cast<UInventory_ItemWidget>(DragOp->DefaultDragVisual))
    {
        ItemWidget->ItemObject = ItemObj;
        ItemWidget->Refresh();
    }

    if (UInventory_EquipmentTile* EquipTile = Cast<UInventory_EquipmentTile>(DragOp->DefaultDragVisual))
    {
        EquipTile->ItemObject = ItemObj;
        EquipTile->Refresh();
    }

	// GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Green, TEXT("Item Rotated (R key)"));
}