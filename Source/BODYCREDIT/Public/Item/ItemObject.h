// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Materials/MaterialInterface.h"
#include "ItemObject.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UItemObject : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Dimensions;

	FIntPoint GetDimension();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* RotatedIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AItem_Base> ItemClass;

	bool Rotated = false;

	UMaterialInterface* GetIcon();

	TSubclassOf<AItem_Base> GetItemClass();
};
