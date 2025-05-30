// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/AC_InventoryEquipmentComponent.h"

UAC_InventoryEquipmentComponent::UAC_InventoryEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UAC_InventoryEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAC_InventoryEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
