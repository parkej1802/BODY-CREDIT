
#include "Inventory/Inventory_EquipmentWidget.h"
#include "../../Public/Inventory/Inventory_EquipmentTile.h"
#include "Item/ItemObject.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "../../Public/Characters/CNox_Runner.h"
#include "../../Public/Inventory/AC_EquipComponent.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Components/Border.h"

void UInventory_EquipmentWidget::InitEquipment()
{
	TileSize = PlayerCharacter->InventoryComp->InventoryTileSize;
	EquipComp = PlayerCharacter->EquipComp;

	if (EquipComp->EquippedItems.Contains(ItemType)) {
		if (InventoryItemTileWidget)
		{
			InventoryItemTileUI = CreateWidget<UInventory_EquipmentTile>(GetWorld(), InventoryItemTileWidget);
			InventoryItemTileUI->ItemType = ItemType;
			InventoryItemTileUI->EquipMainWidget = this;

			InventoryItemTileUI->SetItem(EquipComp->EquippedItems[ItemType]);

			FIntPoint ItemDim = EquipComp->EquippedItems[ItemType]->GetDimension();
			NewSize = FVector2D(ItemDim.X * TileSize, ItemDim.Y * TileSize);

			UCanvasPanelSlot* BorderSlot = Cast<UCanvasPanelSlot>(Border_Grid->Slot);

			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Canvas_Grid->AddChild(InventoryItemTileUI));
			if (CanvasSlot) {
				//CanvasSlot->SetSize(NewSize);
				CanvasSlot->SetAutoSize(true);
				/*UCanvasPanelSlot* BorderSlot = Cast<UCanvasPanelSlot>(Border_Grid->Slot);
				if (BorderSlot)
				{
					BorderSlot->SetSize(NewSize);
				}*/
			}

		}
	}
}


void UInventory_EquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

int32 UInventory_EquipmentWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 MaxLayer = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	if (UWidgetBlueprintLibrary::IsDragDropping() && DrawDropLocation)
	{
		UDragDropOperation* CurrentOp = UWidgetBlueprintLibrary::GetDragDroppingContent();
		UItemObject* TempItemObject = CurrentOp ? Cast<UItemObject>(CurrentOp->Payload) : nullptr;

		if (!TempItemObject)
		{
			return MaxLayer + 1;
		}

		FLinearColor TintColor = IsValidItemType(TempItemObject) ? FLinearColor(0.f, 1.f, 0.f, 0.3f) : FLinearColor(1.f, 0.f, 0.f, 0.3f);

		FVector2D SlotSize = AllottedGeometry.GetLocalSize();

		FSlateDrawElement::MakeBox(
			OutDrawElements,
			MaxLayer + 1,
			AllottedGeometry.ToPaintGeometry(SlotSize, FSlateLayoutTransform()),
			FCoreStyle::Get().GetBrush("WhiteBrush"),
			ESlateDrawEffect::None,
			TintColor
		);

		return MaxLayer + 2;
	}

	return MaxLayer;
}

FReply UInventory_EquipmentWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

bool UInventory_EquipmentWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemObject* DroppedItem = GetPayLoad(InOperation);

	//if (EquipComp && EquipComp->EquippedItems.Contains(EPlayerPart::Backpack))
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Backpack is equipped!"));
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Backpack is NOT equipped."));
	//}

	if (ItemType == EPlayerPart::Weapon2 && DroppedItem->ItemType == EPlayerPart::Weapon1)
	{
		DroppedItem->ItemType = EPlayerPart::Weapon2;
	}

	if (ItemType == EPlayerPart::Weapon1 && DroppedItem->ItemType == EPlayerPart::Weapon2)
	{
		DroppedItem->ItemType = EPlayerPart::Weapon1;
	}

	if (DroppedItem->ItemType != ItemType || EquipComp->EquippedItems.Contains(ItemType)) {
		DrawDropLocation = false;
		return false;
	}

	if (InventoryItemTileWidget)
	{
		InventoryItemTileUI = CreateWidget<UInventory_EquipmentTile>(GetWorld(), InventoryItemTileWidget);
		InventoryItemTileUI->ItemType = ItemType;
		InventoryItemTileUI->EquipMainWidget = this;
		InventoryItemTileUI->EquipBackpackInventoryComp = DroppedItem->InventoryComp; 
		InventoryItemTileUI->SetItem(DroppedItem);
		EquipComp->EquippedItems.Add(ItemType, DroppedItem);

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Canvas_Grid->AddChild(InventoryItemTileUI));
		if (CanvasSlot) {
			//CanvasSlot->SetSize(NewSize);
			CanvasSlot->SetAutoSize(true);
			/*UCanvasPanelSlot* BorderSlot = Cast<UCanvasPanelSlot>(Border_Grid->Slot);
			if (BorderSlot)
			{
				BorderSlot->SetSize(NewSize);
			}*/
		}
		DrawDropLocation = false;
		EquipComp->IsChanged = true;
		InventoryItemTileUI->IsMoving = false;
		return true;
	}
	EquipComp->IsChanged = true;
	InventoryItemTileUI->IsMoving = false;

	return false;
}

bool UInventory_EquipmentWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return true;
}

void UInventory_EquipmentWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	DrawDropLocation = true;

}

void UInventory_EquipmentWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	DrawDropLocation = false;
}

UItemObject* UInventory_EquipmentWidget::GetPayLoad(class UDragDropOperation* Operation)
{
	if (IsValid(Operation))
	{
		UItemObject* TempItemObject = Cast<UItemObject>(Operation->Payload);
		ItemObject = TempItemObject;
		return ItemObject;
	}
	return nullptr;
}

bool UInventory_EquipmentWidget::IsValidItemType(class UItemObject* TempItemObject) const
{
	if (!TempItemObject || !EquipComp)
	{
		return false;
	}

	bool bIsWeaponSlot = (ItemType == EPlayerPart::Weapon1 || ItemType == EPlayerPart::Weapon2);
	bool bIsWeaponItem = (TempItemObject->ItemType == EPlayerPart::Weapon1 || TempItemObject->ItemType == EPlayerPart::Weapon2);

	if (bIsWeaponSlot && bIsWeaponItem)
	{
		return !EquipComp->EquippedItems.Contains(ItemType);
	}

	bool bTypeMatch = TempItemObject->ItemType == ItemType;

	bool bSlotEmpty = !EquipComp->EquippedItems.Contains(ItemType);

	return bTypeMatch && bSlotEmpty;
}

