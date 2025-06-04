// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject.h"
#include "AC_LootingInventoryComponent.h"
#include "Item/Item_Base.h"


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

FItemSaveData UItemObject::ExportData() const
{
	FItemSaveData Data;
	Data.ID = ID;
	Data.Dimensions = Dimensions;
	Data.bRotated = Rotated;
	Data.StartPosition = StartPosition;
	Data.ItemClass = ItemClass;
	Data.Icon = Icon;
	Data.RotatedIcon = RotatedIcon;
	Data.ItemType = ItemType;
	Data.CurrentIndex = CurrentIndex;

	if (ItemActorOwner.IsValid() && ItemActorOwner->LootInventoryComp)
	{
		TSet<UItemObject*> UniqueItems;
		for (auto& Pair : ItemActorOwner->LootInventoryComp->IndexToObject)
		{
			if (IsValid(Pair.Value))
			{
				FInventoryItemData SubItemData;
				SubItemData.ID = Pair.Value->ID;
				SubItemData.Dimensions = Pair.Value->Dimensions;
				SubItemData.bRotated = Pair.Value->Rotated;
				SubItemData.StartPosition = Pair.Value->StartPosition;
				SubItemData.ItemClass = Pair.Value->ItemClass;
				SubItemData.Icon = Pair.Value->Icon;
				SubItemData.RotatedIcon = Pair.Value->RotatedIcon;
				SubItemData.ItemType = Pair.Value->ItemType;
				SubItemData.CurrentIndex = Pair.Value->CurrentIndex;

				Data.ContainedItems.Add(SubItemData);
			}
		}
	}

	return Data;
}

void UItemObject::ImportData(const FItemSaveData& Data)
{
	ID = Data.ID;
	Dimensions = Data.Dimensions;
	Rotated = Data.bRotated;
	StartPosition = Data.StartPosition;
	ItemClass = Data.ItemClass;
	Icon = Data.Icon;
	RotatedIcon = Data.RotatedIcon;
	ItemType = Data.ItemType;
	CurrentIndex = Data.CurrentIndex;
	ContainedItems = Data.ContainedItems;
}