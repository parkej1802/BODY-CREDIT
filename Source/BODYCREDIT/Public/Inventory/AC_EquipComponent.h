// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory_Tile.h"
#include "Item/ItemObject.h"
#include "Item/ItemDT.h"
#include "AC_EquipComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYCREDIT_API UAC_EquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_EquipComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TMap<EPlayerPart, UItemObject*> EquippedItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TMap<EPlayerPart, class AItem_Base*> EquippedItemActor;

	void EquipItem(EPlayerPart Part, UItemObject* Item);

	void UnequipItem(EPlayerPart Part);

	bool IsEquip(EPlayerPart Part);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;

	void SetHasBackpack();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentChanged);

	UPROPERTY(BlueprintAssignable)
	FOnEquipmentChanged EquipmentChanged;

	UFUNCTION()
	void OnEquipmentChanged();
	
	UPROPERTY()
	bool IsChanged = false;

	UPROPERTY()
	class UItemObject* BackpackItem;

	UItemObject* CreateItemFromData(const FItemSaveData& Data);

	bool IsStatChanged = false;

	void SetPlayerStat(class UItemObject* Item, int32 Direction);


};
