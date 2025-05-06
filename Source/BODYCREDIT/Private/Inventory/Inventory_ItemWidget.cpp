// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory_ItemWidget.h"
#include "Item/ItemObject.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Materials/MaterialInterface.h"
#include "Styling/SlateBrush.h"

void UInventory_ItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!IsValid(ItemObject)) return;
	FIntPoint IntPoint = ItemObject->GetDimension();
	Size.X = IntPoint.X * TileSize;
	Size.Y = IntPoint.Y * TileSize;

	SizeBox_BackGround->SetWidthOverride(Size.X);
	SizeBox_BackGround->SetHeightOverride(Size.Y);

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image_Item->Slot);
	CanvasSlot->SetSize(Size);
}

FSlateBrush UInventory_ItemWidget::GetIconImage()
{
	UMaterialInterface* Material = ItemObject->GetIcon();

	FSlateBrush Brush;
	Brush.SetResourceObject(Material);

	Brush.ImageSize = FVector2D(Size.X, Size.Y);

	return Brush;
}

//void UInventory_ItemWidget::OnRemoved(UItemObject* IObject)
//{
//	OnItemRemoved.Broadcast(IObject);
//}
