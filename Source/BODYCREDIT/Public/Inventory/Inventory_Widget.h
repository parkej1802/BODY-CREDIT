﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory_Widget.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UInventory_Widget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_GridWidget* InventoryGridWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_GridWidget* InventoryItemGridWidget;

	UPROPERTY(EditAnywhere)
	class UAC_InventoryComponent* InventoryComp;

	UPROPERTY(EditAnywhere)
	class UAC_LootingInventoryComponent* LootingInventoryComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float TileSize = 50.f;

	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	bool bIsLootable = false;
};
