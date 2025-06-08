// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/ItemObject.h"
#include "AC_LootingInventoryComponent.h"
#include "Item/Item_Base.h"
#include "Item/Functions/ItemStrategy.h"


FIntPoint UItemObject::GetDimension()
{
	FIntPoint Reverse;
	if (ItemData.Rotated) {
		Reverse.X = ItemData.Dimensions.Y;
		Reverse.Y = ItemData.Dimensions.X;
		return Reverse;
	}
	return ItemData.Dimensions;
}

UMaterialInterface* UItemObject::GetIcon()
{
	if (ItemData.Rotated)
	{
		return ItemData.RotatedIcon;
	}
	return ItemData.Icon;
}

TSubclassOf<AItem_Base> UItemObject::GetItemClass()
{
	return ItemData.ItemClass;
}

void UItemObject::Rotate()
{
	ItemData.Rotated = !ItemData.Rotated;
}

bool UItemObject::IsRotated()
{
	return ItemData.Rotated;
}

FIntPoint UItemObject::GetStartPosition()
{
	return ItemData.StartPosition;
}

FItemSaveData UItemObject::ExportData() const
{
	FItemSaveData Data;
	Data.ID = ItemData.ID;
	Data.Dimensions = ItemData.Dimensions;
	Data.bRotated = ItemData.Rotated;
	Data.StartPosition = ItemData.StartPosition;
	Data.ItemClass = ItemData.ItemClass;
	Data.Icon = ItemData.Icon;
	Data.RotatedIcon = ItemData.RotatedIcon;
	Data.ItemType = ItemData.ItemType;
	Data.CurrentIndex = CurrentIndex;

	if (ItemActorOwner.IsValid() && ItemActorOwner->LootInventoryComp)
	{
		TSet<UItemObject*> UniqueItems;
		for (auto& Pair : ItemActorOwner->LootInventoryComp->IndexToObject)
		{
			if (IsValid(Pair.Value))
			{
				FInventoryItemData SubItemData;
				SubItemData.ID = Pair.Value->ItemData.ID;
				SubItemData.Dimensions = Pair.Value->ItemData.Dimensions;
				SubItemData.bRotated = Pair.Value->ItemData.Rotated;
				SubItemData.StartPosition = Pair.Value->ItemData.StartPosition;
				SubItemData.ItemClass = Pair.Value->ItemData.ItemClass;
				SubItemData.Icon = Pair.Value->ItemData.Icon;
				SubItemData.RotatedIcon = Pair.Value->ItemData.RotatedIcon;
				SubItemData.ItemType = Pair.Value->ItemData.ItemType;
				SubItemData.CurrentIndex = Pair.Value->CurrentIndex;

				Data.ContainedItems.Add(SubItemData);
			}
		}
	}


	return Data;
}

void UItemObject::ImportData(const FItemSaveData& Data)
{
	ItemData.ID = Data.ID;
	ItemData.Dimensions = Data.Dimensions;
	ItemData.Rotated = Data.bRotated;
	ItemData.StartPosition = Data.StartPosition;
	ItemData.ItemClass = Data.ItemClass;
	ItemData.Icon = Data.Icon;
	ItemData.RotatedIcon = Data.RotatedIcon;
	ItemData.ItemType = Data.ItemType;
	CurrentIndex = Data.CurrentIndex;
	ContainedItems = Data.ContainedItems;
}

void UItemObject::SetItemStrategy(class UItemStrategy* NewStrategy)
{
	if (ItemStrategy != NewStrategy) {
		ItemStrategy = NewStrategy;
	}
}

void UItemObject::UseItem()
{
	if (ItemStrategy)
	{
		ItemStrategy->Use(this);
	}
}
