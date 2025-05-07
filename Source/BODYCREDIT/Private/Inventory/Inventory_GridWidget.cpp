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


void UInventory_GridWidget::InitInventory(class UAC_InventoryComponent* InventoryComponent, float Inventoy_TileSize)
{
	InventoryComp = InventoryComponent;
	TileSize = Inventoy_TileSize;
	InventoryRows = InventoryComp->Rows;
	InventoryColumns = InventoryComp->Columns;

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border_Grid->Slot);
	CanvasSlot->SetSize(FVector2D(InventoryColumns * TileSize, InventoryRows * TileSize));

	CreateLineSegment();

	Refresh();
	
	InventoryComp->InventoryChanged.AddDynamic(this, &UInventory_GridWidget::Refresh);
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

	for (const FInventoryLine& Line : Lines)
	{
		TArray<FVector2D> Points;
		Points.Add(Line.Start);
		Points.Add(Line.End);

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			Points,
			ESlateDrawEffect::None,
			FLinearColor::White,
			true,
			1.0f
		);
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

	TMap<UItemObject*, FInventoryTile> AllItem = InventoryComp->GetAllItems();

	for (auto& Item : AllItem)
	{
		FInventoryTile& InvenTile = *AllItem.Find(Item.Key);
		if (InventoryItemWidget)
		{
			InventoryItemUI = CreateWidget<UInventory_ItemWidget>(GetWorld(), InventoryItemWidget);
			InventoryItemUI->OnItemRemoved.AddDynamic(this, &UInventory_GridWidget::OnItemRemoved);
			InventoryItemUI->TileSize = TileSize;
			InventoryItemUI->ItemObject = Item.Key;

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
	InventoryComp->RemoveItem(ItemObject);
}

UItemObject* UInventory_GridWidget::GetPayLoad(UDragDropOperation* Operation)
{
	if (IsValid(Operation))
	{
		UItemObject* ItemObject = Cast<UItemObject>(Operation->Payload);
		return ItemObject;
	}
	return nullptr;
}

bool UInventory_GridWidget::IsRoomAvailableForPayload(UItemObject* ItemObject) const
{
	if (IsValid(ItemObject))
	{
		FInventoryTile TempTile;
		TempTile.X = DraggedItemTopLeftTile.X;
		TempTile.Y = DraggedItemTopLeftTile.Y;

		return InventoryComp->IsRoomAvailable(ItemObject, InventoryComp->TileToIndex(TempTile));
	}
	
	return false;
}

TPair<bool, bool> UInventory_GridWidget::MousePositionInTile(FVector2D MousePosition)
{
	bool bX = fmod(MousePosition.X, TileSize) > (TileSize / 2);
	bool bY = fmod(MousePosition.Y, TileSize) > (TileSize / 2);

	return TPair<bool, bool>(bX, bY);
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

	return true;
}

void UInventory_GridWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	DrawDropLocation = true;
}

void UInventory_GridWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	DrawDropLocation = false;
}

bool UInventory_GridWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemObject* ItemObject = GetPayLoad(InOperation);

	if (IsRoomAvailableForPayload(ItemObject))
	{
		FInventoryTile TempTile;
		TempTile.X = DraggedItemTopLeftTile.X;
		TempTile.Y = DraggedItemTopLeftTile.Y;
		InventoryComp->AddItemAt(GetPayLoad(InOperation), InventoryComp->TileToIndex(TempTile));
		return true;
		
	}
	else
	{
		if (!InventoryComp->TryAddItem(ItemObject))
		{
			AGameState_BodyCredit* MyGameState = GetWorld()->GetGameState<AGameState_BodyCredit>();

			MyGameState->SpawnItemFromActor(ItemObject, InventoryComp->GetOwner(), true);
		}
	}
	
	return false;

}
