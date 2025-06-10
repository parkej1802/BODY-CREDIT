// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_Selection.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_Selection : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Play;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_Market;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_WorkShop;

	UFUNCTION()
	void OnPlayClicked();

	UFUNCTION()
	void OnMarketClicked();

	UFUNCTION()
	void OnWorkShopClicked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> LobbyPlayWidgetClass;

	UPROPERTY()
	class UCLobbyWidget_Play* LobbyWidget_Play;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> LobbyMarketWidgetClass;

	UPROPERTY()
	class ULobbyWidget_Market* LobbyWidget_Market;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> LobbyWorkShopWidgetClass;

	UPROPERTY()
	class ULobbyWidget_WorkShop* LobbyWidget_WorkShop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerController* PC;

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Debt;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Humanity;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Power;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Health;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Speed;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Weight;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Energy;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Buff;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Debuff;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_DayCount;

	UPROPERTY()
	class UNetGameInstance* GI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;
};
