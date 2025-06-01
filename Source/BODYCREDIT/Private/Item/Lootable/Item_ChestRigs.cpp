// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Lootable/Item_ChestRigs.h"
#include "AC_LootingInventoryComponent.h"

AItem_ChestRigs::AItem_ChestRigs()
{
	LootInventoryComp = CreateDefaultSubobject<UAC_LootingInventoryComponent>(TEXT("EquipInventoryComp"));
	ItemName = FName("ChestRigs");
}

void AItem_ChestRigs::BeginPlay()
{
	Super::BeginPlay();

	ItemObject->ItemActorOwner = this;
	ItemObject->ItemActorOwner->LootInventoryComp = LootInventoryComp;
}
