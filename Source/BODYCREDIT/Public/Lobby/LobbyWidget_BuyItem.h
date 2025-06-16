// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_BuyItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmBuy, UItemObject*, ItemObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCancelBuy, UItemObject*, ItemObject);

UCLASS()
class BODYCREDIT_API ULobbyWidget_BuyItem : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Confirm;

	UFUNCTION()
	void OnConfirmClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Cancel;

	UFUNCTION()
	void OnCancelClicked();

	UFUNCTION()
	void OnConfirmHovered();

	UFUNCTION()
	void OnConfirmUnhovered();

	UFUNCTION()
	void OnCancelHovered();

	UFUNCTION()
	void OnCancelUnhovered();

	//UPROPERTY(meta = (BindWidgetAnim), Transient)
	//class UWidgetAnimation* Anim_Hovered_Continue;

	//UPROPERTY(meta = (BindWidgetAnim), Transient)
	//class UWidgetAnimation* Anim_Unhovered_Continue;

	//UPROPERTY(meta = (BindWidgetAnim), Transient)
	//class UWidgetAnimation* Anim_Hovered_Exit;

	//UPROPERTY(meta = (BindWidgetAnim), Transient)
	//class UWidgetAnimation* Anim_Unhovered_Exit;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Cancel_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Confirm_Hovered;

	UPROPERTY(BlueprintAssignable)
	FOnConfirmBuy OnConfirmBuy;

	UPROPERTY(BlueprintAssignable)
	FOnCancelBuy OnCancelBuy;

	UFUNCTION()
	void HandleConfirmClicked();

	UFUNCTION()
	void HandleCancelClicked();

	UPROPERTY()
	class UItemObject* ItemObjectBuy;

	UFUNCTION(BlueprintCallable)
	void SetItemToBuy(class UItemObject* ItemObject);

	UPROPERTY()
	class ULobbyWidget_Market* MarketUI;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_ItemPrice;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_RemaingGold;

	void SetUIGold();

	void Refresh();

	UPROPERTY()
	class APlayerController* PC;
};
