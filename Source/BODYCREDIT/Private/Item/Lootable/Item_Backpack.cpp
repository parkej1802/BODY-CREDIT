// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Lootable/Item_Backpack.h"
#include "Item/Functions/Function_Backpack.h"
#include "AC_LootingInventoryComponent.h"
#include "Inventory/AC_InventoryEquipmentComponent.h"

AItem_Backpack::AItem_Backpack()
{
	LootInventoryComp = CreateDefaultSubobject<UAC_LootingInventoryComponent>(TEXT("EquipInventoryComp"));
}

void AItem_Backpack::BeginPlay()
{
	Super::BeginPlay();

	ItemStrategy = NewObject<UFunction_Backpack>();

	ItemObject->ItemActorOwner->LootInventoryComp = LootInventoryComp;
}

