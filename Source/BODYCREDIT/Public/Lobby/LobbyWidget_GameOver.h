// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_GameOver.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_GameOver : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_NewGame;

	UFUNCTION()
	void OnNewGameClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Exit;

	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnNewGameHovered();

	UFUNCTION()
	void OnNewGameUnhovered();

	UFUNCTION()
	void OnExitHovered();

	UFUNCTION()
	void OnExitUnhovered();

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Hovered_NewGame;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Unhovered_NewGame;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Hovered_Exit;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Unhovered_Exit;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Hovered_NewGame_Loop;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Hovered_Exit_Loop;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_NewGame_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Exit_Hovered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerController* PC;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_BackGround;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	class UWidgetAnimation* Anim_Background;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_GameOver1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LobbyMainWidgetClass;

	UPROPERTY()
	class ULobbyWidget_Main* LobbyMainUI;
};
