// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory_EquipmentTile.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"
#include "Inventory/Inventory_EquipmentWidget.h"
#include "Inventory/AC_EquipComponent.h"
#include "Components/SizeBox.h"
#include "Characters/CNox_Runner.h"

void UInventory_EquipmentTile::NativeConstruct()
{
}

FSlateBrush UInventory_EquipmentTile::GetIconImage()
{
	ItemObject->ItemData.Rotated = false;
	UMaterialInterface* Material = ItemObject->GetIcon();

	FSlateBrush Brush;
	Brush.SetResourceObject(Material);
	/*if (ItemObject && EquipMainWidget)
	{
		Brush.ImageSize = NewSize;
	}
	else
	{
		Brush.ImageSize = FVector2D(BorderSize);
	}*/

	Brush.ImageSize = FVector2D(BorderSize);
	return Brush;
}


void UInventory_EquipmentTile::Refresh()
{
	if (!IsValid(ItemObject)) return;

	FIntPoint ItemDim = ItemObject->GetDimension();
	NewSize = FVector2D(ItemDim.X * EquipMainWidget->TileSize, ItemDim.Y * EquipMainWidget->TileSize);

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image_Item->Slot))
	{
		CanvasSlot->SetSize(NewSize);
	}

	FSlateBrush IconBrush;
	IconBrush.SetResourceObject(ItemObject->GetIcon());
	IconBrush.ImageSize = NewSize;

	Image_Item->SetBrush(IconBrush);
}

void UInventory_EquipmentTile::SetItem(UItemObject* NewItem)
{
	if (NewItem->ItemData.ItemType != ItemType) return;

	ItemObject = NewItem;

	if (ItemObject && EquipMainWidget)
	{
		FIntPoint ItemDim = ItemObject->GetDimension();
		NewSize = FVector2D(ItemDim.X * EquipMainWidget->TileSize, ItemDim.Y * EquipMainWidget->TileSize);
		UCanvasPanelSlot* BorderSlot = Cast<UCanvasPanelSlot>(EquipMainWidget->Border_Grid->Slot);

		BorderSize = BorderSlot->GetSize();

		/*SizeBox_BackGround->SetWidthOverride(NewSize.X);
		SizeBox_BackGround->SetHeightOverride(NewSize.Y);*/
		
		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image_Item->Slot);
		// CanvasSlot->SetSize(NewSize);
		CanvasSlot->SetSize(BorderSize);

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
	if (IsMoving) return;
	IsMoving = true;

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDragDropOperation* DragOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	if (!DragOperation) return;


	SizeBox_BackGround->SetWidthOverride(NewSize.X);
	SizeBox_BackGround->SetHeightOverride(NewSize.Y);

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image_Item->Slot))
	{
		CanvasSlot->SetSize(NewSize);
	}

	if (Image_Item)
	{
		FSlateBrush IconBrush;
		IconBrush.SetResourceObject(ItemObject->GetIcon());
		IconBrush.ImageSize = NewSize;
		Image_Item->SetBrush(IconBrush);
	}
	

	DragOperation->DefaultDragVisual = this;
	DragOperation->Pivot = EDragPivot::CenterCenter;
	DragOperation->Payload = ItemObject;

	RemoveFromParent();

	EquipMainWidget->PlayerCharacter->EquipComp->UnequipItem(ItemType);
	
	EquipMainWidget->PlayerCharacter->EquipComp->IsChanged = true;
	OutOperation = DragOperation;
}
