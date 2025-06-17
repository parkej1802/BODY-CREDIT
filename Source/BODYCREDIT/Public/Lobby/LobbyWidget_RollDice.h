// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_RollDice.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_RollDice : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_RollDice;

	UFUNCTION()
	void OnRollDiceClicked();

	UFUNCTION()
	void OnRollDiceHovered();

	UFUNCTION()
	void OnRollDiceUnhovered();

	void RollDice();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_RollDice_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_RollDice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LobbyDayLeftWidgetClass;

	UPROPERTY()
	class ULobbyWidget_DayLeft* LobbyWidget_DayLeft;

	UPROPERTY()
	class UNetGameInstance* GI;
};
