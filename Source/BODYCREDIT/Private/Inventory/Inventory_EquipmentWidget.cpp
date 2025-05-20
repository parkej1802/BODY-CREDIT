
#include "Inventory/Inventory_EquipmentWidget.h"
#include "../../Public/Inventory/Inventory_EquipmentTile.h"
#include "Item/ItemObject.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CanvasPanel.h"
#include "../../Public/Characters/CNox_Runner.h"
#include "../../Public/Inventory/AC_EquipComponent.h"

void UInventory_EquipmentWidget::NativeConstruct()
{
	Super::NativeConstruct();


	APlayerController* PC = GetOwningPlayer();

	APawn* Pawn = PC->GetPawn();

	PlayerCharacter = Cast<ACNox_Runner>(Pawn);

	EquipComp = PlayerCharacter->EquipComp;

	if (EquipComp->EquippedItems.Contains(ItemType)) {
		if (InventoryItemTileWidget)
		{
			InventoryItemTileUI = CreateWidget<UInventory_EquipmentTile>(GetWorld(), InventoryItemTileWidget);
			InventoryItemTileUI->ItemType = ItemType;
			InventoryItemTileUI->EquipMainWidget = this;

			InventoryItemTileUI->SetItem(EquipComp->EquippedItems[ItemType]);

			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Canvas_Grid->AddChild(InventoryItemTileUI));

			if (CanvasSlot) {
				CanvasSlot->SetAutoSize(true);
			}
		}
	}
}

FReply UInventory_EquipmentWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

bool UInventory_EquipmentWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemObject* DroppedItem = GetPayLoad(InOperation);

	if (DroppedItem->ItemType != ItemType) return false;

	if (EquipComp->EquippedItems.Contains(ItemType)) return false;

	if (InventoryItemTileWidget)
	{
		InventoryItemTileUI = CreateWidget<UInventory_EquipmentTile>(GetWorld(), InventoryItemTileWidget);
		InventoryItemTileUI->ItemType = ItemType;
		InventoryItemTileUI->EquipMainWidget = this;
		InventoryItemTileUI->SetItem(DroppedItem);
		EquipComp->EquippedItems.Add(ItemType, DroppedItem);

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Canvas_Grid->AddChild(InventoryItemTileUI));

		if (CanvasSlot) {
			CanvasSlot->SetAutoSize(true);
			// CanvasSlot->SetPosition(FVector2D(100.f, 100.f));
		}

		return true;
	}

	return false;
}

bool UInventory_EquipmentWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

	return false;
}

void UInventory_EquipmentWidget::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

}

void UInventory_EquipmentWidget::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

}

UItemObject* UInventory_EquipmentWidget::GetPayLoad(class UDragDropOperation* Operation)
{
	if (IsValid(Operation))
	{
		UItemObject* TempItemObject = Cast<UItemObject>(Operation->Payload);
		ItemObject = TempItemObject;
		return TempItemObject;
	}
	return nullptr;
}