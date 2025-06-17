// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Lootable/Item_ChestRigs.h"
#include "AC_LootingInventoryComponent.h"
#include "Item/Functions/Function_Backpack.h"

AItem_ChestRigs::AItem_ChestRigs()
{
	LootInventoryComp = CreateDefaultSubobject<UAC_LootingInventoryComponent>(TEXT("EquipInventoryComp"));
}

void AItem_ChestRigs::BeginPlay()
{
	Super::BeginPlay();

	ItemStrategy = NewObject<UFunction_Backpack>();

	ItemObject->ItemActorOwner->LootInventoryComp = LootInventoryComp;

}
