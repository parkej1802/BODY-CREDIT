// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Components/CWeaponComponent.h"
#include "ItemDT.generated.h"

USTRUCT(BlueprintType)
struct FItemRarityIcon
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* BlankIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* CommonIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* RareIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* EpicIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* LegendaryIcon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* BlankRotatedIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* CommonRotatedIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* RareRotatedIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* EpicRotatedIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* LegendaryRotatedIcon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* BlankThumbnail = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* CommonThumbnail = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* RareThumbnail = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* EpicThumbnail = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
    UMaterialInterface* LegendaryThumbnail = nullptr;
};


USTRUCT(BlueprintType)
struct FItemRarityImages
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UTexture2D* BlankImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UTexture2D* CommonImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UTexture2D* RareImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UTexture2D* EpicImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rarity")
	UTexture2D* LegendaryImage = nullptr;
};

USTRUCT(BlueprintType)
struct FItemStatIncrease
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Health = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Strength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Stamina = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MoveSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Armor = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Accuracy = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float CarryWeight = 0.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Humanity = 0.f;


};

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

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    Blank UMETA(DisplayName = "Blank"),
    Common UMETA(DisplayName = "Common"),
    Rare UMETA(DisplayName = "Rare"),
    Epic UMETA(DisplayName = "Epic"),
    Legendary UMETA(DisplayName = "Lengendary")
};

// UENUM(BlueprintType)
// enum class EImage : uint8
// {
// 	
// };

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
    FItemRarityImages RarityImages;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Price;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FItemRarityIcon IconStruct;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UMaterialInterface* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UMaterialInterface* RotatedIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    class UMaterialInterface* EquipedThumbnail;

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

     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
     class USkeletalMesh* SkeletalMesh;

     UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
     FItemStatIncrease StatIncrease;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EWeaponType WeaponType;

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
        , SkeletalMesh(nullptr)
        , StatIncrease()
        , ItemName()
    {
    }
};
