// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/Inventory_Line.h"
#include "Inventory_GridWidget.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UInventory_GridWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Border_Grid;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* Canvas_Grid;


// Widget Functions
public:
	void InitInventory(class UAC_InventoryBaseComponent* InventoryComponent, float Inventoy_TileSize);

	void CreateLineSegment();

// Inventory Data
public:
	UPROPERTY()
    UAC_InventoryBaseComponent* InventoryBaseComp;

	float TileSize = 50.f;

	int32 InventoryRows = 0;
	int32 InventoryColumns = 0;
	int32 LootRows = 0;
	int32 LootColumns = 0;

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

	FIntPoint DraggedItemTopLeftTile;
	bool DrawDropLocation = false;

	TPair<bool, bool> MousePositionInTile(FVector2D MousePosition);


public:
	UPROPERTY(EditAnywhere, Category = Widget)
	TSubclassOf<UUserWidget> InventoryItemWidget;

	class UInventory_ItemWidget* InventoryItemUI;

public:

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual FReply  NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
};
