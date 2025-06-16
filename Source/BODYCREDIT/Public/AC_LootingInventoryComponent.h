// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/Inventory_Tile.h"
#include "Inventory/AC_InventoryBaseComponent.h"
#include "AC_LootingInventoryComponent.generated.h"

UENUM(BlueprintType)
enum class ELootBoxTier : uint8
{
	Blank    UMETA(DisplayName = "Blank"),
	Common    UMETA(DisplayName = "Common"),
	Rare  UMETA(DisplayName = "Rare"),
	Epic      UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYCREDIT_API UAC_LootingInventoryComponent : public UAC_InventoryBaseComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_LootingInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;

	void AddRandomItem();

	void RefreshInventory();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELootBoxTier LootBoxTier = ELootBoxTier::Common;
};
