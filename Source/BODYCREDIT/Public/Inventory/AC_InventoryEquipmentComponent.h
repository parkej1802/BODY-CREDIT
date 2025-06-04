// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory/AC_InventoryBaseComponent.h"
#include "AC_InventoryEquipmentComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYCREDIT_API UAC_InventoryEquipmentComponent : public UAC_InventoryBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAC_InventoryEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
};
