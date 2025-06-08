// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory_ItemInventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UInventory_ItemInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_ItemStrategy* InventoryStrategyUI;


	
};
