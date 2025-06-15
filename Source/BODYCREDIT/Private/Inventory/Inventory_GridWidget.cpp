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
#include "Inventory/Inventory_EquipmentTile.h"
#include "Lobby/LobbyWidget_Market.h"
#include "Games/CMainGM.h"
#include "Characters/CNox_Runner.h"


void UInventory_GridWidget::InitInventory(UAC_InventoryBaseComponent* InventoryComponent, float Inventoy_TileSize)
{
	InventoryBaseComp = InventoryComponent;
	InventoryRows = InventoryBaseComp->Rows; 
	InventoryColumns = InventoryBaseComp->Columns;
	TileSize = Inventoy_TileSize;

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border_Grid->Slot);
	CanvasSlot->SetSize(FVector2D(InventoryColumns * TileSize, InventoryRows * TileSize));

	CreateLineSegment();

	Refresh();
	
	if (!InventoryBaseComp->InventoryChanged.IsAlreadyBound(this, &UInventory_GridWidget::Refresh)) {
		InventoryBaseComp->InventoryChanged.AddDynamic(this, &UInventory_GridWidget::Refresh);
	}
}	

//void UInventory_GridWidget::InitEquipment(UAC_InventoryBaseComponent* InventoryComponent, float Equipment_TileSize)
//{
//	InventoryBaseComp = InventoryComponent;
//	InventoryRows = 1;
//	InventoryColumns = 1;
//	TileSize = Equipment_TileSize;
//
//	IsEquipment = true;
//
//	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border_Grid->Slot);
//	CanvasSlot->SetSize(FVector2D(InventoryColumns * Equipment_TileSize, InventoryRows * Equipment_TileSize));
//
//	CreateLineSegment();
//
//	Refresh();
//
//	InventoryBaseComp->InventoryChanged.AddDynamic(this, &UInventory_GridWidget::Refresh);
//}

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

	if (Lines.Num() > 0) 
	{

		FVector2D GridSize = FVector2D(InventoryColumns * TileSize, InventoryRows * TileSize);
		FVector2f GridSizeF = FVector2f(GridSize);

		FPaintGeometry GridGeom = AllottedGeometry.ToPaintGeometry(
			GridSizeF,
			FSlateLayoutTransform(FVector2f::ZeroVector)
		);

		for (const FInventoryLine& Line : Lines)
		{
			TArray<FVector2D> Points;
			Points.Add(Line.Start);
			Points.Add(Line.End);

			FSlateDrawElement::MakeLines(
				OutDrawElements,
				LayerId + 1,
				GridGeom,
				Points,
				ESlateDrawEffect::None,
				FLinearColor::White,
				true,
				1.0f
			);
		}
	

		if (!IsCurrentlyHovered()) {
			return LayerId + 1;
		}

		if (UWidgetBlueprintLibrary::IsDragDropping() && DrawDropLocation)
		{
			UDragDropOperation* CurrentOp = UWidgetBlueprintLibrary::GetDragDroppingContent();
			UItemObject* ItemObject = CurrentOp ? Cast<UItemObject>(CurrentOp->Payload) : nullptr;

			if (!ItemObject) {
				return LayerId + 1;
			}

			FLinearColor PossibleTint = IsRoomAvailableForPayload(ItemObject) ? FLinearColor(0.f, 1.f, 0.f, 0.3f) : FLinearColor(1.f, 0.f, 0.f, 0.3f);

			FVector2D BoxPosition = FVector2D(DraggedItemTopLeftTile.X * TileSize, DraggedItemTopLeftTile.Y * TileSize);
			FIntPoint Dim = ItemObject->GetDimension();
			FVector2D BoxSize = FVector2D(Dim.X * TileSize, Dim.Y * TileSize);

			FPaintGeometry BoxGeom = AllottedGeometry.ToPaintGeometry(
				FVector2f(BoxSize),
				FSlateLayoutTransform(FVector2f(BoxPosition))
			);

			FSlateDrawElement::MakeBox(
				OutDrawElements,
				LayerId + 1,
				BoxGeom,
				FCoreStyle::Get().GetBrush("WhiteBrush"),
				ESlateDrawEffect::None,
				PossibleTint
			);
		}
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
			if (!InventoryItemUI->OnItemRemoved.IsAlreadyBound(this, &UInventory_GridWidget::OnItemRemoved)) {
				InventoryItemUI->OnItemRemoved.AddDynamic(this, &UInventory_GridWidget::OnItemRemoved);
			}
			InventoryItemUI->TileSize = TileSize;
			InventoryItemUI->ItemObject = Item.Key;

			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Canvas_Grid->AddChild(InventoryItemUI));

			if (CanvasSlot) {
				CanvasSlot->SetAutoSize(true);
				CanvasSlot->SetZOrder(10);
				CanvasSlot->SetPosition(FVector2D(Item.Value.X * TileSize, Item.Value.Y * TileSize));
			}
		}
	}
}

void UInventory_GridWidget::OnItemRemoved(UItemObject* ItemObject)
{
	if (!IsCurrentlyHovered() || !ItemObject) return;
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
	//if (OwningMarketInventoryWidget) {
	//	return true;
	//}

	//if (!OwningInventoryWidget || !OwningInventoryWidget->CurrentHoveredGrid) return false;
	//// GEngine->AddOnScreenDebugMessage(2, 2.0f, FColor::Red, FString::Printf(TEXT("Currently Hovered %d"), OwningInventoryWidget->CurrentHoveredGrid->GridID));
	//return OwningInventoryWidget->CurrentHoveredGrid == this;
	return CurrentlyHovered;
}

void UInventory_GridWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	CurrentlyHovered = true;
}
void UInventory_GridWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	CurrentlyHovered = false;
}

//bool UInventory_GridWidget::IsCurrentlyHovered() const
//{
//	if (!OwningInventoryWidget || !OwningInventoryWidget->CurrentHoveredGrid)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("No hovered grid or OwningInventoryWidget is null"));
//		return false;
//	}
//
//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Currently Hovered GridID: %d, This GridID: %d"),
//		OwningInventoryWidget->CurrentHoveredGrid->GridID, GridID));
//
//	bool bIsHovered = (OwningInventoryWidget->CurrentHoveredGrid == this);
//
//	if (!bIsHovered)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("This widget is NOT currently hovered"));
//	}
//
//	return bIsHovered;
//}

FGeometry UInventory_GridWidget::GetGridContentGeometry()
{
	if (Canvas_Grid)
	{
		return Canvas_Grid->GetCachedGeometry();
	}

	return GetCachedGeometry();
}

void UInventory_GridWidget::ClearInventory()
{
	Lines.Empty();
	Canvas_Grid->ClearChildren();
	InventoryBaseComp = nullptr;
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
	if (!ItemObject) {
		return false;
	}
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
	CurrentlyHovered = true;
	DrawDropLocation = true;
}

void UInventory_GridWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//if (!IsCurrentlyHovered()) return;
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	CurrentlyHovered = false;
	DrawDropLocation = false;
}

void UInventory_GridWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

bool UInventory_GridWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (UInventory_ItemWidget* DraggedWidget = Cast<UInventory_ItemWidget>(InOperation->DefaultDragVisual))
	{
		DraggedWidget->IsMoving = false;
	}

	if (UInventory_EquipmentTile* InventoryItemTileUI = Cast<UInventory_EquipmentTile>(InOperation->DefaultDragVisual)) 
	{
		InventoryItemTileUI->IsMoving = false;
	}

	if (!IsCurrentlyHovered()) return false;

	UItemObject* ItemObject = GetPayLoad(InOperation);
	if (!ItemObject) return false;

	FInventoryTile TempTile;

	if (IsRoomAvailableForPayload(ItemObject))
	{
		TempTile.X = DraggedItemTopLeftTile.X;
		TempTile.Y = DraggedItemTopLeftTile.Y;
		InventoryBaseComp->AddItemAt(ItemObject, InventoryBaseComp->TileToIndex(TempTile));
	}
	else
	{
		TempTile.X = ItemObject->ItemData.StartPosition.X;
		TempTile.Y = ItemObject->ItemData.StartPosition.Y;

		if (!InventoryBaseComp->TryAddItem(ItemObject))
		{
			if (Cast<ACMainGM>(GetWorld()->GetAuthGameMode())->IsStart)
			{
				Cast<AGameState_BodyCredit>(GetWorld()->GetGameState())->SpawnItemFromActor(ItemObject, PlayerController->GetPawn(), true);
			}
			else
			{
				ACNox_Runner* PlayerCharacter = Cast<ACNox_Runner>(PlayerController->GetPawn());
				PlayerCharacter->InventoryComp->TryAddItem(ItemObject);
			}
		}
	}

	InventoryBaseComp->OnInventoryChanged();
	
	return true;

}
