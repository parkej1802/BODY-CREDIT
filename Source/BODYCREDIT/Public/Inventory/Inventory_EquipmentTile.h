// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Layout/Geometry.h"
#include "Item/ItemObject.h"
#include "Inventory_EquipmentTile.generated.h"

/**
 * 
 */
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, UItemObject*, RemovedItem);

UCLASS()
class BODYCREDIT_API UInventory_EquipmentTile : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY()
	FIntPoint DraggedItemTopLeftTile;
	UPROPERTY()
	bool DrawDropLocation = false;

	TPair<bool, bool> MousePositionInTile(FVector2D MousePosition);

	bool IsCurrentlyHovered() const;

	UPROPERTY(EditAnywhere)
	class UAC_EquipComponent* EquipComp;

	UPROPERTY(EditAnywhere)
	class UInventory_EquipmentWidget* EquipMainWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USizeBox* SizeBox_BackGround;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Border_BackGround;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Item;

	UPROPERTY()
	class UItemObject* ItemObject;

	//UPROPERTY(EditAnywhere)
	//class UAC_LootingInventoryComponent* EquipBackpackInventoryComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerPart ItemType;

	UPROPERTY()
	FVector2D BorderSize;

	UPROPERTY()
	FVector2D NewSize;

	UPROPERTY()
	FVector2D Size;

	FSlateBrush GetIconImage();

	virtual void NativeConstruct() override;

	void SetItem(class UItemObject* NewItem);

	UPROPERTY()
	bool IsMoving = false;

	void Refresh();

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
};
