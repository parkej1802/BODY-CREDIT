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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> LobbySelectionWidgetClass;

	UPROPERTY()
	class ULobbyWidget_Selection* LobbyWidget_Selection;
};
