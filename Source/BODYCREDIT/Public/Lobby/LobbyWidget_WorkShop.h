// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_WorkShop.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_WorkShop : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Back;

	UFUNCTION()
	void OnBackClicked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> LobbySelectionWidgetClass;

	UPROPERTY()
	class ULobbyWidget_Selection* LobbyWidget_Selection;

	UPROPERTY(EditAnywhere)
	class UAC_InventoryComponent* InventoryComp;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_GridWidget* InventoryGridWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Controller* PC;

	UPROPERTY(EditAnywhere)
	class UAC_EquipComponent* EquipComp;

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
	class UInventory_EquipmentWidget* Equip_Backpack_1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_EquipmentWidget* Equip_ChestRigs_1;

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
	class UBorder* Border_ChestRigs_1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* Border_Backpack_1;

	UPROPERTY(EditAnywhere)
	class UAC_LootingInventoryComponent* EquipBackpackInventoryComp;

	UPROPERTY(EditAnywhere)
	class UAC_LootingInventoryComponent* EquipChestRigsInventoryComp;
	
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_GridWidget* InventoryEquipBackpackGridWidget_1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UInventory_GridWidget* InventoryEquipChestRigsGridWidget_1;

	UPROPERTY()
	class UItemObject* BackpackItem;

	UPROPERTY()
	class UItemObject* ChestRigsItem;

	UFUNCTION()
	void SetItemInventory();
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UPlayer3DOverlayWidget* Player3DOverlayWidget;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

};
