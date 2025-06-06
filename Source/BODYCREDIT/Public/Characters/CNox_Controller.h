// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CNox_Controller.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API ACNox_Controller : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACNox_Controller();

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputAction* IA_RotateItem;

	void SetupInputComponent();

	void RotateDraggedItem();
};
