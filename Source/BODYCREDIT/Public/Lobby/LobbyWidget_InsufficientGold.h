﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_InsufficientGold.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_InsufficientGold : public UUserWidget
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

	//UPROPERTY(meta = (BindWidgetAnim), Transient)
	//class UWidgetAnimation* Anim_Hovered_Confirm;

	//UPROPERTY(meta = (BindWidgetAnim), Transient)
	//class UWidgetAnimation* Anim_Unhovered_Confirm;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Confirm_Hovered;
};
