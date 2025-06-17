// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_Market.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_Market : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
// Back
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

// Weapon
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_SelectWeapon;

	UFUNCTION()
	void OnSelectWeaponClicked();

	UFUNCTION()
	void OnSelectWeaponHovered();

	UFUNCTION()
	void OnSelectWeaponUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_SelectWeapon_Hovered;

// Head
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_SelectHead;

	UFUNCTION()
	void OnSelectHeadClicked();

	UFUNCTION()
	void OnSelectHeadHovered();

	UFUNCTION()
	void OnSelectHeadUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_SelectHead_Hovered;

// Body
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_SelectBody;

	UFUNCTION()
	void OnSelectBodyClicked();

	UFUNCTION()
	void OnSelectBodyHovered();

	UFUNCTION()
	void OnSelectBodyUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_SelectBody_Hovered;

// Arm
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_SelectArm;

	UFUNCTION()
	void OnSelectArmClicked();

	UFUNCTION()
	void OnSelectArmHovered();

	UFUNCTION()
	void OnSelectArmUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_SelectArm_Hovered;

// Leg
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_SelectLeg;

	UFUNCTION()
	void OnSelectLegClicked();

	UFUNCTION()
	void OnSelectLegHovered();

	UFUNCTION()
	void OnSelectLegUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_SelectLeg_Hovered;

// Backpack
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_SelectBackpack;

	UFUNCTION()
	void OnSelectBackpackClicked();

	UFUNCTION()
	void OnSelectBackpackHovered();

	UFUNCTION()
	void OnSelectBackpackUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_SelectBackpack_Hovered;

// ChestRig
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_SelectChestrig;

	UFUNCTION()
	void OnSelectChestrigClicked();

	UFUNCTION()
	void OnSelectChestrigHovered();

	UFUNCTION()
	void OnSelectChestrigUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_SelectChestrig_Hovered;

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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UScrollBox* ScrollBox_Market;

	UPROPERTY()
	class UAC_MarketComponent* MarketComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> MarketItemWidget;

	UPROPERTY()
	class UMarket_ItemTile* MarketItemUI;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* VerticalBox_MarketItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D ImageSize = FVector2D(150, 100);

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_PlayerGold;

	UPROPERTY()
	class UNetGameInstance* GI;

	UFUNCTION()
	void UpdatePlayerGoldText(int32 NewGold);
	 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> SellItemWidget;

	UPROPERTY()
	class ULobbyWidget_SellItem* SellItemUI;

	void ShowSellUI(class UItemObject* ItemObject);

	UFUNCTION()
	void HandleSellConfirm(class UItemObject* ItemObject);

	UFUNCTION()
	void HandleSellCancel(class UItemObject* ItemObject);

	UPROPERTY()
	class UImage* PreviousImage;

	void TurnOffPreviousImage();

	void TurnOnPreviousImage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> BuyItemWidget;

	UPROPERTY()
	class ULobbyWidget_BuyItem* BuyItemUI;

	void ShowBuyUI(class UItemObject* ItemObject);

	UFUNCTION()
	void HandleBuyConfirm(class UItemObject* ItemObject);

	UFUNCTION()
	void HandleBuyCancel(class UItemObject* ItemObject);

	UPROPERTY()
	class AGameState_BodyCredit* GameState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> NoSpaceWidget;

	UPROPERTY()
	class ULobbyWidget_NoSpace* NoSpaceUI;

	void ShowNoSpaceUI();

	void Refresh();

	UFUNCTION()
	void RemoveWidget();
};
