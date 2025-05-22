// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory_GridWidget.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "Item/ItemObject.h"
#include "Inventory/Inventory_ItemWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameState_BodyCredit.h"
#include "Blueprint/DragDropOperation.h"
#include "AC_LootingInventoryComponent.h"
#include "Inventory/AC_InventoryBaseComponent.h"
#include "Inventory/Inventory_Widget.h"


void UInventory_GridWidget::InitInventory(class UAC_InventoryBaseComponent* InventoryComponent, float Inventoy_TileSize)
{
	InventoryBaseComp = InventoryComponent;
	InventoryRows = InventoryBaseComp->Rows;
	InventoryColumns = InventoryBaseComp->Columns;
	TileSize = Inventoy_TileSize;

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border_Grid->Slot);
	CanvasSlot->SetSize(FVector2D(InventoryColumns * Inventoy_TileSize, InventoryRows * Inventoy_TileSize));

	CreateLineSegment();

	Refresh();
	
	InventoryBaseComp->InventoryChanged.AddDynamic(this, &UInventory_GridWidget::Refresh);
}	

void UInventory_GridWidget::InitEquipment(UAC_InventoryBaseComponent* InventoryComponent, float Equipment_TileSize)
{
	InventoryBaseComp = InventoryComponent;
	InventoryRows = 1;
	InventoryColumns = 1;
	TileSize = Equipment_TileSize;

	IsEquipment = true;

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border_Grid->Slot);
	CanvasSlot->SetSize(FVector2D(InventoryColumns * Equipment_TileSize, InventoryRows * Equipment_TileSize));

	CreateLineSegment();

	Refresh();

	InventoryBaseComp->InventoryChanged.AddDynamic(this, &UInventory_GridWidget::Refresh);
}

void UInventory_GridWidget::CreateLineSegment()
{
	int32 LocalX = 0;
	int32 LocalY = 0;
	FInventoryLine TempLine;

	for (int32 i = 0; i <= InventoryColumns; ++i) {
		LocalX = TileSize * i;
		
		TempLine.Start.X = LocalX;
		TempLine.Start.Y = 0;
		TempLine.End.X = LocalX;
		TempLine.End.Y = InventoryRows * TileSize;

		Lines.Add(TempLine);
	}

	for (int32 i = 0; i <= InventoryRows; ++i) {
		LocalY = TileSize * i;

		TempLine.Start.X = 0;
		TempLine.Start.Y = LocalY;
		TempLine.End.X = InventoryColumns * TileSize;
		TempLine.End.Y = LocalY;

		Lines.Add(TempLine);
	}
}

int32 UInventory_GridWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	if (!IsEquipment) {
		FVector2D GridSize(InventoryColumns * TileSize, InventoryRows * TileSize);

		FPaintGeometry GridGeom = AllottedGeometry.ToPaintGeometry(
			FVector2D::ZeroVector,
			GridSize
		);

		for (const FInventoryLine& Line : Lines)
		{
			TArray<FVector2D> Points;
			Points.Add(Line.Start);
			Points.Add(Line.End);

			FSlateDrawElement::MakeLines(
				OutDrawElements,
				LayerId,
				GridGeom,
				Points,
				ESlateDrawEffect::None,
				FLinearColor::White,
				true,
				1.0f
			);
		}

	}

	if (!IsCurrentlyHovered()) {
		return LayerId + 1;
	}

	if (UWidgetBlueprintLibrary::IsDragDropping() && DrawDropLocation)
	{
		UDragDropOperation* CurrentOp = UWidgetBlueprintLibrary::GetDragDroppingContent();
		UItemObject* ItemObject = CurrentOp ? Cast<UItemObject>(CurrentOp->Payload) : nullptr;

		if (!ItemObject)
		{
			return LayerId + 1;
		}

		FLinearColor PossibleTint = IsRoomAvailableForPayload(ItemObject) ? FLinearColor(0.f, 1.f, 0.f, 0.3f) : FLinearColor(1.f, 0.f, 0.f, 0.3f);

		FVector2D BoxPosition = FVector2D(DraggedItemTopLeftTile.X * TileSize, DraggedItemTopLeftTile.Y * TileSize);

		FIntPoint Dim = ItemObject->GetDimension();
		FVector2D BoxSize = FVector2D(Dim.X * TileSize, Dim.Y * TileSize);

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			LayerId + 1,
			AllottedGeometry.ToPaintGeometry(BoxPosition, BoxSize),
			FCoreStyle::Get().GetBrush("WhiteBrush"),
			ESlateDrawEffect::None,
			PossibleTint
		);
	}
	return LayerId + 2;

}

void UInventory_GridWidget::Refresh()
{
	Canvas_Grid->ClearChildren();

	 TMap<UItemObject*, FInventoryTile> AllItem = InventoryBaseComp->GetAllItems();
	// TArray<TPair<UItemObject*, FInventoryTile>> AllItem = InventoryBaseComp->GetAllItems();
	// TMap<int32, FInventoryTile> AllItem = InventoryBaseComp->GetAllItems();

	for (auto& Item : AllItem)
	{
		// FInventoryTile& InvenTile = *AllItem.Find(Item.Key);
		if (InventoryItemWidget)
		{
			InventoryItemUI = CreateWidget<UInventory_ItemWidget>(GetWorld(), InventoryItemWidget);
			InventoryItemUI->OnItemRemoved.AddDynamic(this, &UInventory_GridWidget::OnItemRemoved);
			InventoryItemUI->TileSize = TileSize;
			InventoryItemUI->ItemObject = Item.Key;
			//InventoryItemUI->ItemObject = InventoryBaseComp->IndexToObject[Item.Key];
			InventoryItemUI->MainInventoryWidget = OwningInventoryWidget;

			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Canvas_Grid->AddChild(InventoryItemUI));

			if (CanvasSlot) {
				CanvasSlot->SetAutoSize(true);
				CanvasSlot->SetPosition(FVector2D(Item.Value.X * TileSize, Item.Value.Y * TileSize));
			}
		}
	}
}

void UInventory_GridWidget::OnItemRemoved(UItemObject* ItemObject)
{
	if (!IsCurrentlyHovered()) return;
	InventoryBaseComp->RemoveItem(ItemObject);
}

UItemObject* UInventory_GridWidget::GetPayLoad(UDragDropOperation* Operation)
{
	if (!IsCurrentlyHovered()) return nullptr;
	if (IsValid(Operation))
	{
		UItemObject* ItemObject = Cast<UItemObject>(Operation->Payload);
		return ItemObject;
	}
	return nullptr;
}

bool UInventory_GridWidget::IsRoomAvailableForPayload(UItemObject* ItemObject) const
{
	if (!IsCurrentlyHovered()) return false;
	if (IsValid(ItemObject))
	{
		FInventoryTile TempTile;
		TempTile.X = DraggedItemTopLeftTile.X;
		TempTile.Y = DraggedItemTopLeftTile.Y;

		return InventoryBaseComp->IsRoomAvailable(ItemObject, InventoryBaseComp->TileToIndex(TempTile));
	}
	
	return false;
}

TPair<bool, bool> UInventory_GridWidget::MousePositionInTile(FVector2D MousePosition)
{
	bool bX = fmod(MousePosition.X, TileSize) > (TileSize / 2);
	bool bY = fmod(MousePosition.Y, TileSize) > (TileSize / 2);

	return TPair<bool, bool>(bX, bY);
}

bool UInventory_GridWidget::IsCurrentlyHovered() const
{
	if (!OwningInventoryWidget || !OwningInventoryWidget->CurrentHoveredGrid) return false;
	//GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Red, FString::Printf(TEXT("Currently Hovered %d"), OwningInventoryWidget->CurrentHoveredGrid->GridID));
	return OwningInventoryWidget->CurrentHoveredGrid == this;
}



FGeometry UInventory_GridWidget::GetGridContentGeometry()
{
	if (Canvas_Grid)
	{
		return Canvas_Grid->GetCachedGeometry();
	}

	return GetCachedGeometry();
}

FReply UInventory_GridWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
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



bool UInventory_GridWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

	if (!IsValid(InOperation)) {
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("NativeOnDragOver : %s"), GridID));
		return false;
	}

	bool Right;
	bool Down;

	FVector2D MousePosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
	TPair<bool, bool> bResult = MousePositionInTile(MousePosition);
	Right = bResult.Key;
	Down = bResult.Value;

	UItemObject* ItemObject = Cast<UItemObject>(InOperation->Payload);
	FIntPoint TempDimension = ItemObject->GetDimension();

	int32 AdjustedX = Right ? FMath::Clamp(TempDimension.X - 1, 0, TempDimension.X - 1) : FMath::Clamp(TempDimension.X, 0, TempDimension.X);
	int32 AdjustedY = Down ? FMath::Clamp(TempDimension.Y - 1, 0, TempDimension.Y - 1) : FMath::Clamp(TempDimension.Y, 0, TempDimension.Y);

	FIntPoint ResultIntPoint(AdjustedX, AdjustedY);

	ResultIntPoint /= 2;

	FVector2D TempResult = MousePosition / TileSize;
	TempResult.X = FMath::TruncToInt(TempResult.X);
	TempResult.Y = FMath::TruncToInt(TempResult.Y);

	DraggedItemTopLeftTile = FIntPoint(TempResult.X, TempResult.Y) - ResultIntPoint;

	return false;
}

void UInventory_GridWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//if (!IsCurrentlyHovered()) return;
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	DrawDropLocation = true;
}

void UInventory_GridWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//if (!IsCurrentlyHovered()) return;
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	DrawDropLocation = false;
}

void UInventory_GridWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

bool UInventory_GridWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (!IsCurrentlyHovered()) return false;

	UItemObject* ItemObject = GetPayLoad(InOperation);
	FInventoryTile TempTile;
	if (IsRoomAvailableForPayload(ItemObject))
	{
		TempTile.X = DraggedItemTopLeftTile.X;
		TempTile.Y = DraggedItemTopLeftTile.Y;
		InventoryBaseComp->AddItemAt(GetPayLoad(InOperation), InventoryBaseComp->TileToIndex(TempTile));
		
	}
	else
	{
		TempTile.X = ItemObject->StartPosition.X;
		TempTile.Y = ItemObject->StartPosition.Y;

		InventoryBaseComp->TryAddItem(ItemObject);
	}
	
	return true;

}
