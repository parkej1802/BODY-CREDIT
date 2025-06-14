// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_Failed.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_Failed : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Continue;

	UFUNCTION()
	void OnContinueClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Exit;

	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnContinueHovered();

	UFUNCTION()
	void OnContinueUnhovered();

	UFUNCTION()
	void OnExitHovered();

	UFUNCTION()
	void OnExitUnhovered();

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Hovered_Continue;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Unhovered_Continue;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Hovered_Exit;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Unhovered_Exit;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Hovered_Continue_Loop;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Hovered_Exit_Loop;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Continue_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Exit_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Background;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_BackGround;

	UPROPERTY()
	class UNetGameInstance* GI;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_LostPart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Controller* PC;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> LobbySelectionWidgetClass;

	UPROPERTY()
	class ULobbyWidget_Selection* LobbyWidget_Selection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> LobbyGameOverWidgetClass;

	UPROPERTY()
	class ULobbyWidget_GameOver* LobbyWidget_LobbyGameOver;

	void Refresh();
};
