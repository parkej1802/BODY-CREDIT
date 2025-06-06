// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory_ItemStrategy.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UInventory_ItemStrategy::NativeConstruct()
{
	Super::NativeConstruct();
}

FReply UInventory_ItemStrategy::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UInventory_ItemStrategy::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	
}
