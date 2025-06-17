// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget_EscapeTimer.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ULobbyWidget_EscapeTimer : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
	float EscapeTimer = 0.f;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Text_Timer;

	bool IsVisible = false; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;

	bool IsDead = false;

	UPROPERTY()
	class UNetGameInstance* GI;
};
