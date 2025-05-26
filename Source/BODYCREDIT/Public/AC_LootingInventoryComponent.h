// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory/Inventory_Tile.h"
#include "Inventory/AC_InventoryBaseComponent.h"
#include "AC_LootingInventoryComponent.generated.h"


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

public:
	
};
