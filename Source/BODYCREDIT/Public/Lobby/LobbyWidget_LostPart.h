// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_LostPart.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_LostPart : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Continue;

	UFUNCTION()
	void OnContinueClicked();

	UFUNCTION()
	void OnContinueHovered();

	UFUNCTION()
	void OnContinueUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Continue_Hovered;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_LostPart;



};
