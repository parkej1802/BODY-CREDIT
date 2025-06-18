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

	float TileSize = 75.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UItemObject* ItemObject;

	UPROPERTY()
	FVector2D Size;

	FSlateBrush GetIconImage();

	//UPROPERTY()
 //   class UInventory_Widget* MainInventoryWidget;

    UPROPERTY(BlueprintAssignable)
    FOnItemRemoved OnItemRemoved;

	UFUNCTION(BlueprintCallable)
    void CallOnRemoved();

	void Refresh();

	virtual void NativeConstruct() override;

	UPROPERTY()
	FIntPoint StartTilePosition;

	UPROPERTY()
	bool IsMoving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> ItemMenuWidget;

	UPROPERTY()
	class ULobbyWidget_ItemMenu* ItemMenuUI;

	UFUNCTION()
	void RemoveWidget();

	UPROPERTY()
	class UNetGameInstance* GI;

public:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

};

