// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Sniper.h"
#include "Item/ItemObject.h"

AItem_Sniper::AItem_Sniper()
{
	/*ConstructorHelpers::FObjectFinder<UMaterialInterface>Temp_Mat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Item/Icons/M_Icon_Weapon.M_Icon_Weapon'"));
	if (Temp_Mat.Succeeded())
	{
		Icon = Temp_Mat.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterialInterface>Temp_MatR(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Item/Icons/M_Icon_WeaponRotated.M_Icon_WeaponRotated'"));
	if (Temp_MatR.Succeeded())
	{
		RotatedIcon = Temp_MatR.Object;
	}*/

	ItemName = "Sniper";
}

//UItemObject* AItem_Sniper::GetDefaultItemObject()
//{
//	UItemObject* NewItem = NewObject<UItemObject>(this, UItemObject::StaticClass());
//
//	NewItem->Dimensions.X = Width;
//	NewItem->Dimensions.Y = Height;
//	NewItem->Icon = Icon;
//	NewItem->RotatedIcon = RotatedIcon;
//	NewItem->ItemClass = GetClass();
//
//	return NewItem;
//}
