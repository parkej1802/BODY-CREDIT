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
	class UCanvasPanel* Canvas_Grid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;

	UPROPERTY(EditAnywhere, Category = Widget)
	TSubclassOf<UUserWidget> InventoryItemTileWidget;

	class UInventory_EquipmentTile* InventoryItemTileUI;

	UPROPERTY(EditAnywhere)
	class UAC_EquipComponent* EquipComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerPart ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UItemObject* ItemObject;

public:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
