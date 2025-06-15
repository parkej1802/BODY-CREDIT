// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_DayLeft.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_DayLeft : public UUserWidget
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

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Confirm_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_DayLeft;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UHorizontalBox* HorizontalBox_DayLeft;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_PayAmount;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LobbyPlayWidgetClass;

	UPROPERTY()
	class UCLobbyWidget_Play* LobbyWidget_Play;

	void Refresh();

	int32 DayLeft = 1;

	UPROPERTY()
	class UNetGameInstance* GI;
};
