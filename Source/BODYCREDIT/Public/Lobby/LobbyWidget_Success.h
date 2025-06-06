// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_Success.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_Success : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Continue;

	UFUNCTION()
	void OnContinueClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Continue_Hovered;

	UFUNCTION()
	void OnContinueHovered();

	UFUNCTION()
	void OnContinueUnhovered();

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Hovered_Continue_Loop;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Hovered_Continue;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Unhovered_Continue;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_BackGround;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_BackGround;
};
