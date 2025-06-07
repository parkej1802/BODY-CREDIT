// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory_ItemStrategy.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UInventory_ItemStrategy : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_GridWidget* ItemInventoryGridWidget;

	UPROPERTY(EditAnywhere)
	class UAC_InventoryBaseComponent* InventoryComp;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Exit;

	UFUNCTION()
	void OnExitClicked();

	
	UPROPERTY(EditAnywhere, meta = (BindWidget))

	class UCanvasPanel* CanvasPanel_ItemStrategy;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Drag;

	UFUNCTION()
	void OnDragPressed();

	
	UFUNCTION()
	void OnDragReleased();


	FVector2D DragOffset;

	FVector2D MouseOffset;
	bool bIsDragging = false;

	UPROPERTY()
	class UInventory_ItemInventoryWidget* ParentWidget;

	UPROPERTY()
	class UItemObject* ItemObject;
	

	virtual void NativeConstruct() override;
	/*virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;*/
	//virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	//virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
