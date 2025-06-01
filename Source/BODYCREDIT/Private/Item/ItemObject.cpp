// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject.h"
#include "AC_LootingInventoryComponent.h"


FIntPoint UItemObject::GetDimension()
{
	FIntPoint Reverse;
	if (Rotated) {
		Reverse.X = Dimensions.Y;
		Reverse.Y = Dimensions.X;
		return Reverse;
	}
	return Dimensions;
}

UMaterialInterface* UItemObject::GetIcon()
{
	if (Rotated)
	{
		return RotatedIcon;
	}
	return Icon;
}

TSubclassOf<AItem_Base> UItemObject::GetItemClass()
{
	return ItemClass;
}

void UItemObject::Rotate()
{
	Rotated = !Rotated;
}

bool UItemObject::IsRotated()
{
	return Rotated;
}

FIntPoint UItemObject::GetStartPosition()
{
	return StartPosition;
}

