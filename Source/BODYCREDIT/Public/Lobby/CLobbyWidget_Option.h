// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CLobbyWidget_Option.generated.h"

class UButton;
class USlider;
class UWidgetAnimation;
class UNetGameInstance;

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCLobbyWidget_Option : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UNetGameInstance* GameInstance;
	
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_Back;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* LobbySoundSlider;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* InGameSlider;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* ExtractSoundSlider;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* StartOptionTextAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* StartOptionAnim;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* StartBackAnim;

	UFUNCTION()
	void OnBackButtonCliced();
	UFUNCTION()
	void OnLobbySoundVolumeChange(float Value);
	UFUNCTION()
	void OnLobbySoundCaptureEnd();
	UFUNCTION()
	void OnInGameVolumeChange(float Value);
	UFUNCTION()
	void OnExtractSoundVolumeChange(float Value);

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> LobbyMainWidgetClass;

	UPROPERTY()
	class ULobbyWidget_Main* LobbyMainUI;
};
