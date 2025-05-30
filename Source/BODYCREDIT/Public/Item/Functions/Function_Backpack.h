// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Functions/ItemStrategy.h"
#include "Function_Backpack.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UFunction_Backpack : public UItemStrategy
{
	GENERATED_BODY()
	
public:
	virtual void Use(class AItem_Base* Item) override;
};
