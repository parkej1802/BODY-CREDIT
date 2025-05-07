// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory_ItemWidget.generated.h"

/**
 * 
 */


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemRemoved, UItemObject*, RemovedItem);

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

    UPROPERTY(BlueprintAssignable)
    FOnItemRemoved OnItemRemoved;

	UFUNCTION(BlueprintCallable)
    void CallOnRemoved();

	virtual void NativeConstruct() override;

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
 
};

