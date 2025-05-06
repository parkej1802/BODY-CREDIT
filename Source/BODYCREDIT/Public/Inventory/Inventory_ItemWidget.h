// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory_ItemWidget.generated.h"

/**
 * 
 */

UCLASS()
class BODYCREDIT_API UInventory_ItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USizeBox* SizeBox_BackGround;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Border_BackGround;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Item;

	float TileSize = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UItemObject* ItemObject;

	FVector2D Size;

	FSlateBrush GetIconImage();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, UItemObject*, RemovedItem);

    UPROPERTY(BlueprintAssignable)
    FOnItemRemoved OnItemRemoved;

	/*UFUNCTION()
    void OnRemoved(class UItemObject* IObject);*/

	virtual void NativeConstruct() override;
};
