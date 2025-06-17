// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory_ItemWidget.h"
#include "Item/ItemObject.h"
#include "Components/SizeBox.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Materials/MaterialInterface.h"
#include "Styling/SlateBrush.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Inventory/Inventory_Widget.h"
#include "Item/Item_Base.h"
#include "Lobby/LobbyWidget_ItemMenu.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/Engine.h"
#include "Session/NetGameInstance.h"

void UInventory_ItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Refresh();

	if (UNetGameInstance* GI = Cast<UNetGameInstance>(GetGameInstance()))
	{
		GI->OnBack.RemoveAll(this);
		GI->OnBack.AddUObject(this, &UInventory_ItemWidget::RemoveWidget);
	}
	
}

FSlateBrush UInventory_ItemWidget::GetIconImage()
{
	UMaterialInterface* Material = ItemObject->GetIcon();

	FSlateBrush Brush;
	Brush.SetResourceObject(Material);
	Brush.ImageSize = FVector2D(FMath::TruncToInt(Size.X), FMath::TruncToInt(Size.Y));

	return Brush;
}

void UInventory_ItemWidget::CallOnRemoved()
{
	if (!ItemObject) return;
	OnItemRemoved.Broadcast(ItemObject);
}


void UInventory_ItemWidget::Refresh()
{
	if (!IsValid(ItemObject)) return;
	FIntPoint IntPoint = ItemObject->GetDimension();
	Size.X = FMath::RoundToInt(IntPoint.X * TileSize);
	Size.Y = FMath::RoundToInt(IntPoint.Y * TileSize);

	SizeBox_BackGround->SetWidthOverride(Size.X);
	SizeBox_BackGround->SetHeightOverride(Size.Y);

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image_Item->Slot);
	CanvasSlot->SetSize(Size);
	CanvasSlot->SetZOrder(10);

	if (Image_Item)
	{
		FSlateBrush IconBrush = GetIconImage();
		Image_Item->SetBrush(IconBrush);
	}
}

void UInventory_ItemWidget::RemoveWidget()
{
	if (ItemMenuUI)
	{
		ItemMenuUI->RemoveFromParent();
		ItemMenuUI = nullptr;
	}
}

FReply UInventory_ItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (ItemMenuUI) {
			ItemMenuUI->RemoveFromParent();
			ItemMenuUI = nullptr;
		}
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (ItemObject->bIsMenu)
		{
			ItemObject->bIsMenu = !ItemObject->bIsMenu;
			if (ItemMenuUI) {
				ItemMenuUI->RemoveFromParent();
				ItemMenuUI = nullptr;
			}
			
			return FReply::Handled();
		}

		if (!ItemObject->bIsMenu && ItemMenuWidget)
		{
			ItemMenuUI = CreateWidget<ULobbyWidget_ItemMenu>(this, ItemMenuWidget);
			ItemMenuUI->ItemObject = ItemObject;
		}
		if (ItemMenuUI)
		{
			ItemMenuUI->AddToViewport(100);

			const FGeometry& Geometry = GetCachedGeometry();
			FVector2D AbsoluteScreenPos = Geometry.GetAbsolutePosition();
			float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
			FVector2D DPIAdjustedPosition = AbsoluteScreenPos / ViewportScale;

			const float MenuOffsetX = Size.X + 10.0f;

			FVector2D MenuPosition = DPIAdjustedPosition + FVector2D(MenuOffsetX, 0.f);

			FVector2D ViewportSize;
			GEngine->GameViewport->GetViewportSize(ViewportSize);

			const float MenuWidth = 200.0f;

			if ((MenuPosition.X + MenuWidth) > ViewportSize.X)
			{
				MenuPosition = DPIAdjustedPosition - FVector2D(MenuWidth + 10.0f, 0.f);
			}

			ItemMenuUI->SetPositionInViewport(MenuPosition, false);
			ItemObject->bIsMenu = !ItemObject->bIsMenu;
		}
		
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}


void UInventory_ItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	Border_BackGround->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f, 0.2f));
}

void UInventory_ItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	Border_BackGround->SetBrushColor(FLinearColor(0.0f, 0.f, 0.f, 0.5f));
}

void UInventory_ItemWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (ItemMenuUI) {
		ItemMenuUI->RemoveFromParent();
		ItemMenuUI = nullptr;
	}
	if (IsMoving || ItemObject->bIsUseFunction) return;
	IsMoving = true;

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDragDropOperation* DragOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	if (!DragOperation) return;

	DragOperation->DefaultDragVisual = this;
	DragOperation->Pivot = EDragPivot::CenterCenter;
	DragOperation->Payload = ItemObject;

	if (ItemObject)
	{
		CallOnRemoved();
	}

	RemoveFromParent();

	OutOperation = DragOperation;
}
