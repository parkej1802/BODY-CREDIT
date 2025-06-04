// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemObject.h"
#include "ItemDT.generated.h"

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    Common UMETA(DisplayName = "Common"),
    Rare UMETA(DisplayName = "Rare"),
    Epic UMETA(DisplayName = "Epic")
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    class UStaticMesh* Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FIntPoint Dimensions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Quantity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool Stackable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* Thumbnail;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Index;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemRarity Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Price;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UMaterialInterface* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UMaterialInterface* RotatedIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<class AItem_Base> ItemClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool Rotated = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FIntPoint StartPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EPlayerPart ItemType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float Weight;

    FItemData()
        : ID(0)
        , Mesh(nullptr)
        , Dimensions(1, 1)
        , Quantity(0)
        , Stackable(false)
        , Thumbnail(nullptr)
        , Index(-1)
        , Rarity(EItemRarity::Common)
        , Price(0)
        , Description(TEXT(""))
        , Icon(nullptr)
        , RotatedIcon(nullptr)
        , ItemClass(nullptr)
        , Rotated(false)
        , StartPosition(0, 0)
        , ItemType(EPlayerPart::Basic)
        , Weight(1)
    {
    }
};
