// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory_EquipmentTile.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Inventory/Inventory_EquipmentWidget.h"
#include "Inventory/AC_EquipComponent.h"

void UInventory_EquipmentTile::NativeConstruct()
{
}

FSlateBrush UInventory_EquipmentTile::GetIconImage()
{
	UMaterialInterface* Material = ItemObject->GetIcon();

	FSlateBrush Brush;
	Brush.SetResourceObject(Material);
	Brush.ImageSize = FVector2D(100.f, 100.f);
	return Brush;
}

void UInventory_EquipmentTile::SetItem(UItemObject* NewItem)
{
	if (NewItem->ItemType != ItemType) return;

	ItemObject = NewItem;

	if (ItemObject)
	{
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image_Item->Slot);
		CanvasSlot->SetSize(FVector2D(100.f, 100.f));

		if (Image_Item)
		{
			FSlateBrush IconBrush = GetIconImage();
			Image_Item->SetBrush(IconBrush);
		}
	}
	else
	{
		Image_Item->SetBrush(FSlateBrush());
	}
}

FReply UInventory_EquipmentTile::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}


bool UInventory_EquipmentTile::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	return false;
}

void UInventory_EquipmentTile::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	Border_BackGround->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.2f));
}

void UInventory_EquipmentTile::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	Border_BackGround->SetBrushColor(FLinearColor(0.0f, 0.f, 0.f, 0.5f));
}

void UInventory_EquipmentTile::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDragDropOperation* DragOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	if (!DragOperation) return;

	DragOperation->DefaultDragVisual = this;
	DragOperation->Pivot = EDragPivot::CenterCenter;
	DragOperation->Payload = ItemObject;

	RemoveFromParent();

	EquipMainWidget->EquipComp->EquippedItems.Remove(ItemType);

	OutOperation = DragOperation;
}
