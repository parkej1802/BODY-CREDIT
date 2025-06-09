// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_SellItem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmSell, UItemObject*, ItemObject);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCancelSell, UItemObject*, ItemObject);

UCLASS()
class BODYCREDIT_API ULobbyWidget_SellItem : public UUserWidget
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
	FOnConfirmSell OnConfirmSell;

	UPROPERTY(BlueprintAssignable)
	FOnCancelSell OnCancelSell;

	UFUNCTION()
	void HandleConfirmClicked();

	UFUNCTION()
	void HandleCancelClicked();

	UPROPERTY()
	class UItemObject* ItemObjctSell;

	UFUNCTION(BlueprintCallable)
	void SetItemToSell(class UItemObject* ItemObject);

	UPROPERTY()
	class ULobbyWidget_Market* MarketUI;
};
