﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Materials/MaterialInterface.h"
#include "ItemObject.generated.h"

UENUM(BlueprintType)
enum class EPlayerPart : uint8
{
	Basic	 UMETA(DisplayName = "Basic"),
    Head     UMETA(DisplayName = "Head"),
    Body     UMETA(DisplayName = "Body"),
	Arm		 UMETA(DisplayName = "Arm"),
	Leg      UMETA(DisplayName = "Leg"),
    Weapon1   UMETA(DisplayName = "Weapon1"),
	Weapon2  UMETA(DisplayName = "Weapon2"),
	Backpack	UMETA(DisplayName = "Backpack"),
	ChestRigs	UMETA(DisplayName = "ChestRigs")
};


USTRUCT(BlueprintType)
struct FInventoryItemData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Dimensions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRotated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint StartPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AItem_Base> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* RotatedIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerPart ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentIndex;
};

USTRUCT(BlueprintType)
struct FItemSaveData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Dimensions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRotated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint StartPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AItem_Base> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* RotatedIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerPart ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryItemData> ContainedItems;
};


UCLASS(BlueprintType, Blueprintable)
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

	//UPROPERTY()
	//class UAC_LootingInventoryComponent* ItemObjectInventoryComp;

	UPROPERTY()
	bool Rotated = false;

	//UPROPERTY()
	//TWeakObjectPtr<class AItem_Backpack> ItemActorOwner;

	
	UPROPERTY()
	TWeakObjectPtr<class AItem_Base> ItemActorOwner;

	UMaterialInterface* GetIcon();

	TSubclassOf<AItem_Base> GetItemClass();

	UPROPERTY()
	FIntPoint StartPosition;

	void Rotate();
	bool IsRotated();
	FIntPoint GetStartPosition();

	int32 CurrentIndex = 0;

	int32 ID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EPlayerPart ItemType = EPlayerPart::Basic;

	FItemSaveData ExportData() const;
	void ImportData(const FItemSaveData& Data);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryItemData> ContainedItems;

};
