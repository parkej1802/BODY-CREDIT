// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject.h"

FIntPoint UItemObject::GetDimension()
{
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
