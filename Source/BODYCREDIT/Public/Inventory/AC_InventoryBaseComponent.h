// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory_Tile.h"
#include "Item/ItemObject.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Inventory/Inventory_ItemWidget.h"
#include "AC_InventoryBaseComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYCREDIT_API UAC_InventoryBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_InventoryBaseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	// Inventory Data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	int32 Columns = 4.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	int32 Rows = 5.f;

	// Item
public:
	bool TryAddItem(class UItemObject* ItemObject);

	bool IsRoomAvailable(class UItemObject* ItemObject, int32 TopLeftIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InventoryTileSize = 75.f;

	FInventoryTile IndexToTile(int32 Index);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UItemObject*> Items;

	void ForEachIndex(class UItemObject* ItemObject, int32 TopLeftIndex);
	bool IsTileValid(FInventoryTile& Tile);
	class UItemObject* GetItemAtIndex(int32 Index);

	int32 TileToIndex(FInventoryTile& Tile);

	void AddItemAt(class UItemObject* ItemObject, int32 TopLeftIndex);

	UPROPERTY()
	bool IsDirty = false;

	TMap<class UItemObject*, FInventoryTile> GetAllItems();
	// TArray<TPair<UItemObject*, FInventoryTile>> GetAllItems();
	// TMap<int32, FInventoryTile> GetAllItems();

	UPROPERTY()
	TMap<int32, class UItemObject*> IndexToObject;

	void RemoveItem(class UItemObject* ItemObject);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

    UPROPERTY(BlueprintAssignable)
	FOnInventoryChanged InventoryChanged;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class AItem_Base* ItemBase;

	UFUNCTION()
	void OnInventoryChanged();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, TSubclassOf<class AItem_Base>> ItemAddMap;

	void PreAddItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACMainGM* GameMode;
};
