// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Functions/Function_Backpack.h"
#include "Item/Lootable/Item_Backpack.h"

void UFunction_Backpack::Use(class AItem_Base* Item)
{
	if (AItem_Backpack* Backpack = Cast<AItem_Backpack>(Item)) {
		
	}
}
