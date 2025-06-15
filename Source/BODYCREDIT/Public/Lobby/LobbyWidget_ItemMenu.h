// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_ItemMenu.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_ItemMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Use;

	UFUNCTION()
	void OnUseClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Sell;

	UFUNCTION()
	void OnSellClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Description;

	UFUNCTION()
	void OnDescriptionClicked();

	UFUNCTION()
	void OnUseHovered();

	UFUNCTION()
	void OnUseUnhovered();

	UFUNCTION()
	void OnSellHovered();

	UFUNCTION()
	void OnSellUnhovered();

	UFUNCTION()
	void OnDescriptionHovered();

	UFUNCTION()
	void OnDescriptionUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Use_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Sell_Hovered;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Description_Hovered;

	UPROPERTY();
	class UItemObject* ItemObject;

	UPROPERTY()
	class UNetGameInstance* GI;

	void ResetInputMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> ItemDescriptionWidgetClass;

	UPROPERTY()
	class 	ULobbyWidget_ItemDescription* ItemDescriptionUI;


};
