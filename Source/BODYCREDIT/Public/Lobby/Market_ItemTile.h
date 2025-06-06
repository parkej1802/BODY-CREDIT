// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemDT.h"
#include "Market_ItemTile.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UMarket_ItemTile : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Item;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_BuyItem;

	UFUNCTION()
	void OnBuyItemClicked();

	UPROPERTY()
	FItemData ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Controller* PC;

	UPROPERTY(EditAnywhere)
	class UAC_InventoryComponent* InventoryComp;

	UPROPERTY()
    class UAC_InventoryBaseComponent* InventoryBaseComp;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
