﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Market_Item.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UMarket_Item : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UImage* Image_Item;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Button_BuyItem;
};
