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
	void InitInventory(class UAC_InventoryComponent* InventoryComponent, float TileSize);

	void CreateLineSegment();

// Inventory Data
public:
	UPROPERTY()
	class UAC_InventoryComponent* InventoryComp;

	float TileSize = 50.f;

	int32 InventoryRows = 0;
	int32 InventoryColumns = 0;

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

	void Refresh();

	void OnItemRemoved(class UItemObject* ItemObject);

public:
	UPROPERTY(EditAnywhere, Category = Widget)
	TSubclassOf<UUserWidget> InventoryItemWidget;

	class UInventory_ItemWidget* InventoryItemUI;

	
};
