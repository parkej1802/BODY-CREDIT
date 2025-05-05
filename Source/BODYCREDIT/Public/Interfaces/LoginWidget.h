// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UButton* Button_CreateRoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UEditableText* Edit_RoomName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class USlider* Slider_PlayerCount;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BindWidget))
	class UTextBlock* Text_PlayerCount;

	UPROPERTY()
	class UNetGameInstance* GI;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void CreateRoom();

	UFUNCTION()
	void OnValueChanged(float Value);

};
