// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/Inventory_Line.h"
#include "Characters/CNox_Controller.h"
#include "Inventory_GridWidget.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UInventory_GridWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/*UPROPERTY()
	class UInventory_Widget* OwningInventoryWidget;

	UPROPERTY()
	class ULobbyWidget_Market* OwningMarketInventoryWidget;

	UPROPERTY()
	class ULobbyWidget_WorkShop* OwningWorkShopInventoryWidget;*/

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Border_Grid;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* Canvas_Grid;

	UPROPERTY(EditAnywhere)
	int32 GridID;
	
	UPROPERTY()
	ACNox_Controller* PlayerController;

	bool CurrentlyHovered = false;

// Widget Functions
public:
	void InitInventory(class UAC_InventoryBaseComponent* InventoryComponent, float Inventoy_TileSize);

	void CreateLineSegment();

// Inventory Data
public:
	UPROPERTY()
    UAC_InventoryBaseComponent* InventoryBaseComp;
	
	UPROPERTY()
	float TileSize = 75.f;

	int32 InventoryRows = 0;
	int32 InventoryColumns = 0;
	int32 LootRows = 0;
	int32 LootColumns = 0;

	UPROPERTY()
	bool IsEquipment = false;

	UPROPERTY()
	TArray<FInventoryLine> Lines;

	virtual int32 NativePaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled
	) const override;

	UFUNCTION()
	void Refresh();

	UFUNCTION()
	void OnItemRemoved(class UItemObject* ItemObject);


	UItemObject* GetPayLoad(class UDragDropOperation* Operation);

	bool IsRoomAvailableForPayload(class UItemObject* ItemObject) const;

	UPROPERTY()
	FIntPoint DraggedItemTopLeftTile;

	UPROPERTY()
	bool DrawDropLocation = false;

	TPair<bool, bool> MousePositionInTile(FVector2D MousePosition);

	bool IsCurrentlyHovered() const;

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
public:
	UPROPERTY(EditAnywhere, Category = Widget)
	TSubclassOf<UUserWidget> InventoryItemWidget;

	UPROPERTY()
	class UInventory_ItemWidget* InventoryItemUI;

	FGeometry GetGridContentGeometry();

	void ClearInventory();

	/*UFUNCTION()
	void RemoveWidget();*/

	UPROPERTY()
	class UNetGameInstance* GI;

public:

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
