// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_Main.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_Main : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_NewGame;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Setting;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Exit;

	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnSettingClicked();

	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnNewGameHovered();

	UFUNCTION()
	void OnNewGameUnhovered();

	UFUNCTION()
	void OnSettingHovered();

	UFUNCTION()
	void OnSettingUnhovered();

	UFUNCTION()
	void OnExitHovered();

	UFUNCTION()
	void OnExitUnhovered();

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Hovered_NewGame;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Unhovered_NewGame;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Hovered_Setting;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Unhovered_Setting;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Hovered_Exit;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Unhovered_Exit;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Start_NewGame;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Start_Setting;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Start_Exit;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_NewGame_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Setting_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Exit_Hovered;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> LobbySelectionWidgetClass;
	
	UPROPERTY()
	class ULobbyWidget_Selection* LobbyWidget_Selection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> OptionWidgetClass;

	UPROPERTY()
	class UCLobbyWidget_Option* LobbyWidget_Option;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerController* PC;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNetGameInstance* GI;

		
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;

};
