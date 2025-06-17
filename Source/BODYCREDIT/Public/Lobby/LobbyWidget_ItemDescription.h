// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_ItemDescription.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_ItemDescription : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Confirm;

	UFUNCTION()
	void OnConfirmClicked();

	UFUNCTION()
	void OnConfirmHovered();

	UFUNCTION()
	void OnConfirmUnhovered();

	void Refresh();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Confirm_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UVerticalBox* VerticalBox_ItemStat;

	UPROPERTY()
	class UItemObject* ItemObject;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Item;

	UPROPERTY()
	class UNetGameInstance* GI;
};
