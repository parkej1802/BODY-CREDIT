// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Functions/ItemStrategy.h"
#include "Function_Backpack.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UFunction_Backpack : public UItemStrategy
{
	GENERATED_BODY()
	
public:
	virtual void Use(class UItemObject* ItemObject) override;

	UPROPERTY(EditAnywhere, Category = Widget)
	TSubclassOf<UUserWidget> InventoryWidget;

	UPROPERTY()
	class UInventory_ItemStrategy* InventoryUI;

	UPROPERTY()
	class UInventory_ItemInventoryWidget* ItemInventoryUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Controller* PC;


};
