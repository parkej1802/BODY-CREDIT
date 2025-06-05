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
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Back;

	UFUNCTION()
	void OnBackClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_SelectWeapon;

	UFUNCTION()
	void OnSelectWeaponClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_SelectHead;

	UFUNCTION()
	void OnSelectHeadClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_SelectBody;

	UFUNCTION()
	void OnSelectBodyClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_SelectArm;

	UFUNCTION()
	void OnSelectArmClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_SelectLeg;

	UFUNCTION()
	void OnSelectLegClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_SelectUtility;

	UFUNCTION()
	void OnSelectUtilityClicked();

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
	class APlayerController* PC;

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
};
