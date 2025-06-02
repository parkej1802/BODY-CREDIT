// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_WorkShop.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_WorkShop : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Back;

	UFUNCTION()
	void OnBackClicked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> LobbySelectionWidgetClass;

	UPROPERTY()
	class ULobbyWidget_Selection* LobbyWidget_Selection;


};
