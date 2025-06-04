// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Item_Base.h"
#include "Item_Backpack.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API AItem_Backpack : public AItem_Base
{
	GENERATED_BODY()
	
public:
	AItem_Backpack();

	virtual void BeginPlay() override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class UAC_LootingInventoryComponent* LootInventoryComp;

	//UPROPERTY(EditAnywhere
	//TSubclassOf<UUserWidget> InventoryBackpackWidget;

	UPROPERTY()
	class UInventory_Widget* InventoryBackpackUI;
};
