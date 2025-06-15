// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_ItemMenu2.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_ItemMenu2 : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Function;

	UFUNCTION()
	void OnFunctionClicked();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Description;

	UFUNCTION()
	void OnDescriptionClicked();

	UFUNCTION()
	void OnFunctionHovered();

	UFUNCTION()
	void OnFunctionUnhovered();

	UFUNCTION()
	void OnDescriptionHovered();

	UFUNCTION()
	void OnDescriptionUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Function_Hovered;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Button_Description_Hovered;

	UPROPERTY();
	class UItemObject* ItemObject;

	UPROPERTY()
	class UNetGameInstance* GI;

	void ResetInputMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> ItemDescriptionWidgetClass;

	UPROPERTY()
	class ULobbyWidget_ItemDescription* ItemDescriptionUI;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Function;

	void Refresh();
};
