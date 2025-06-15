// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_Payment.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_Payment : public UUserWidget
{
	GENERATED_BODY()
	

public:	
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Payment;

	UFUNCTION()
	void OnPaymentClicked();

	UFUNCTION()
	void OnPaymentHovered();

	UFUNCTION()
	void OnPaymentUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Skip;

	UFUNCTION()
	void OnSkipClicked();

	UFUNCTION()
	void OnSkipHovered();

	UFUNCTION()
	void OnSkipUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Payment_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Skip_Hovered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> LobbyRollDiceWidgetClass;

	UPROPERTY()
	class ULobbyWidget_RollDice* LobbyWidget_RollDice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> FailedWidgetClass;

	UPROPERTY()
	class ULobbyWidget_Failed* FailedUI;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_PayAmount;
	
	void Refresh();

	UPROPERTY()
	class UNetGameInstance* GI;
};
