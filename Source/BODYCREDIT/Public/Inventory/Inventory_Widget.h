// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_EquipmentWidget* Equip_Head;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_EquipmentWidget* Equip_Arm;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_EquipmentWidget* Equip_Body;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_EquipmentWidget* Equip_Leg;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_EquipmentWidget* Equip_Weapon_1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_EquipmentWidget* Equip_Weapon_2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_EquipmentWidget* Equip_Backpack;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_EquipmentWidget* Equip_ChestRigs;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Border_Head;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Border_Arm;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Border_Body;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Border_Leg;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Border_Weapon_1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Border_Weapon_2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Border_ChestRigs;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Border_Backpack;

	UPROPERTY(EditAnywhere)
	class UAC_InventoryComponent* InventoryComp;

	UPROPERTY(EditAnywhere)
	class UAC_EquipComponent* EquipComp;

	UPROPERTY(EditAnywhere)
	class UAC_LootingInventoryComponent* LootingInventoryComp;

	UPROPERTY(EditAnywhere)
	class UAC_LootingInventoryComponent* EquipBackpackInventoryComp;

	UPROPERTY(EditAnywhere)
	class UAC_LootingInventoryComponent* EquipChestRigsInventoryComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Controller* PC;

	UPROPERTY()
	class UItemObject* BackpackItem;

	UPROPERTY()
	class UItemObject* ChestRigsItem;

	UFUNCTION()
	void SetItemInventory();

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Power;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Health;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Weight;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Humanity;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Energy;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Speed;

	void PlayerStatChange();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// float TileSize = 50.f;

	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	//UPROPERTY()
	//class UInventory_GridWidget* CurrentHoveredGrid = nullptr;

	UPROPERTY()
	bool bIsLootable = false;

	// void IsMouseOnGrid();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//TSubclassOf<class UInventory_GridWidget> InventoryGridWidgetClass;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_GridWidget* InventoryEquipGridWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_GridWidget* InventoryEquipChestRigsGridWidget;

	bool bHasBackpack = false;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UPlayer3DOverlayWidget* Player3DOverlayWidget;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Back;

	UFUNCTION()
	void OnBackClicked();

	UFUNCTION()
	void OnBackHovered();

	UFUNCTION()
	void OnBackUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Back_Hovered;
};
