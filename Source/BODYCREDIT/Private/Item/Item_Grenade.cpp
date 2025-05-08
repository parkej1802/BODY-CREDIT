// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item_Grenade.h"
#include "Item/ItemObject.h"

AItem_Grenade::AItem_Grenade()
{
	ConstructorHelpers::FObjectFinder<UMaterialInterface>Temp_Mat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Item/Icons/M_Icon_Grenade.M_Icon_Grenade'"));
	if (Temp_Mat.Succeeded())
	{
		Icon = Temp_Mat.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface>Temp_MatR(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Item/Icons/M_Icon_GrenadeRotated.M_Icon_GrenadeRotated'"));
	if (Temp_MatR.Succeeded())
	{
		RotatedIcon = Temp_MatR.Object;
	}

	Width = 1;
	Height = 1;
}

UItemObject* AItem_Grenade::GetDefaultItemObject()
{
	UItemObject* NewItem = NewObject<UItemObject>(this, UItemObject::StaticClass());

	NewItem->Dimensions.X = Width;
	NewItem->Dimensions.Y = Height;
	NewItem->Icon = Icon;
	NewItem->RotatedIcon = RotatedIcon;
	NewItem->ItemClass = GetClass();

	return NewItem;

}

