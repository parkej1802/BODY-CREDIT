// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory_EquipmentTile.h"
#include "../Item/ItemObject.h"
#include "Inventory_EquipmentWidget.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UInventory_EquipmentWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UItemObject* GetPayLoad(class UDragDropOperation* Operation);
	
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Border_Grid;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCanvasPanel* Canvas_Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;

	UPROPERTY(EditAnywhere, Category = Widget)
	TSubclassOf<UUserWidget> InventoryItemTileWidget;

	UPROPERTY()
	class UInventory_EquipmentTile* InventoryItemTileUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerController* PC;

	UPROPERTY(EditAnywhere)
	class UAC_EquipComponent* EquipComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerPart ItemType;

	UPROPERTY()
	class UItemObject* ItemObject;

	UPROPERTY()
	FVector2D NewSize;

	UPROPERTY()
	float TileSize;

	UPROPERTY()
	bool DrawDropLocation = false;

	bool IsValidItemType(class UItemObject* TempItemObject) const;

	void InitEquipment();

public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual int32 NativePaint(
		const FPaintArgs& Args,
		const FGeometry& AllottedGeometry,
		const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements,
		int32 LayerId,
		const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled
	) const override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};
