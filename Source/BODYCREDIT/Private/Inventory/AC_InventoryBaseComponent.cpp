// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/AC_InventoryBaseComponent.h"
#include "Item/Item_Base.h"
#include "Games/CMainGM.h"

// Sets default values for this component's properties
UAC_InventoryBaseComponent::UAC_InventoryBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UAC_InventoryBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Items.SetNum(Columns * Rows);
	if (GetWorld())
	{
		GameMode = Cast<ACMainGM>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			PreAddItem();
		}
	}
}


// Called every frame
void UAC_InventoryBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsDirty)
	{
		IsDirty = false;
		OnInventoryChanged();
	}
}


bool UAC_InventoryBaseComponent::TryAddItem(UItemObject* ItemObject)
{
	if (IsValid(ItemObject))
	{
		for (int32 i = 0; i < Items.Num(); ++i)
		{
			if (IsRoomAvailable(ItemObject, i))
			{
				AddItemAt(ItemObject, i);
				//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("ItemObject Index : %d"), ItemObject->ID));
				return true;
			}
		}
		ItemObject->Rotate();

		for (int32 i = 0; i < Items.Num(); ++i)
		{
			if (IsRoomAvailable(ItemObject, i))
			{
				AddItemAt(ItemObject, i);
				return true;
			}
		}

	}
	return false;
}

FInventoryTile UAC_InventoryBaseComponent::IndexToTile(int32 Index)
{
	FInventoryTile TempTile;

	TempTile.X = Index % Columns;
	TempTile.Y = Index / Columns;

	return TempTile;
}

int32 UAC_InventoryBaseComponent::TileToIndex(FInventoryTile& Tile)
{
	int32 Result;
	return Result = Tile.X + (Tile.Y * Columns);
}

void UAC_InventoryBaseComponent::AddItemAt(class UItemObject* ItemObject, int32 TopLeftIndex)
{
	FInventoryTile ResultTile;

	FInventoryTile TempTile = IndexToTile(TopLeftIndex);
	FIntPoint TempDimension = ItemObject->GetDimension();

	for (int32 i = TempTile.X; i < (TempDimension.X + TempTile.X); ++i)
	{
		for (int32 j = TempTile.Y; j < (TempDimension.Y + TempTile.Y); ++j)
		{
			ResultTile.X = i;
			ResultTile.Y = j;
			if (IsTileValid(ResultTile))
			{
				int32 CurrentIndex = TileToIndex(ResultTile);
				Items[CurrentIndex] = ItemObject;
				IndexToObject.Add(CurrentIndex, ItemObject);
				ItemObject->StartPosition = FIntPoint(TempTile.X, TempTile.Y);

				// GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Green, FString::Printf(TEXT("Item Index %d"), TileToIndex(ResultTile)));
			}
		}
	}
	
	IsDirty = true;
}

bool UAC_InventoryBaseComponent::IsRoomAvailable(class UItemObject* ItemObject, int32 TopLeftIndex)
{
	FInventoryTile ResultTile;

	FInventoryTile TempTile = IndexToTile(TopLeftIndex);
	FIntPoint TempDimension = ItemObject->GetDimension();

	for (int32 i = TempTile.X; i < (TempDimension.X + TempTile.X); ++i)
	{
		for (int32 j = TempTile.Y; j < (TempDimension.Y + TempTile.Y); ++j)
		{
			ResultTile.X = i;
			ResultTile.Y = j;

			if (!IsTileValid(ResultTile))
			{
				return false;
			}

			UItemObject* TempItemObject = GetItemAtIndex(TileToIndex(ResultTile));
			if (IsValid(TempItemObject))
			{
				return false;
			}
		}
	}

	return true;
}

void UAC_InventoryBaseComponent::ForEachIndex(class UItemObject* ItemObject, int32 TopLeftIndex)
{
	FInventoryTile ResultTile;

	FInventoryTile TempTile = IndexToTile(TopLeftIndex);
	FIntPoint TempDimension = ItemObject->GetDimension();

	for (int32 i = TempTile.X; i < (TempDimension.X - 1 + TempTile.X); ++i)
	{
		for (int32 j = TempTile.Y; j < (TempDimension.Y - 1 + TempTile.Y); ++j)
		{
			ResultTile.X = i;
			ResultTile.Y = j;
			IsTileValid(ResultTile);
		}
	}
}

bool UAC_InventoryBaseComponent::IsTileValid(FInventoryTile& Tile)
{
	if (Tile.X >= 0 && Tile.Y >= 0 && Tile.X < Columns && Tile.Y < Rows) {
		return true;
	}

	return false;
}

UItemObject* UAC_InventoryBaseComponent::GetItemAtIndex(int32 Index)
{
	if (Items.IsValidIndex(Index))
	{
		return Items[Index];
	}

	return nullptr;
}

//TMap<int32, FInventoryTile> UAC_InventoryBaseComponent::GetAllItems()
//{
//	TMap<int32, FInventoryTile> AllItem;
//	for (int32 i = 0; i < Items.Num(); ++i)
//	{
//		if (IsValid(Items[i]))
//		{
//			if (!AllItem.Contains(Items[i]->ID))
//			{
//				AllItem.Add(Items[i]->ID, IndexToTile(i));
//			}
//		}
//	}
//	return AllItem;
//}

TMap<UItemObject*, FInventoryTile> UAC_InventoryBaseComponent::GetAllItems()
{
	TMap<UItemObject*, FInventoryTile> AllItem;
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (IsValid(Items[i]))
		{
			if (!AllItem.Contains(Items[i]))
			{
				AllItem.Add(Items[i], IndexToTile(i));
			}
		}
	}
	return AllItem;
}



void UAC_InventoryBaseComponent::RemoveItem(UItemObject* ItemObject)
{
	if (IsValid(ItemObject))
	{
		for (int i = 0; i < Items.Num(); ++i)
		{
			if (ItemObject == Items[i])
			{
				Items[i] = nullptr;
				IsDirty = true;
			}
		}
	}
}

void UAC_InventoryBaseComponent::OnInventoryChanged()
{
	InventoryChanged.Broadcast();
}

void UAC_InventoryBaseComponent::PreAddItem()
{
	for (auto& ItemPair : ItemAddMap)
	{
		AItem_Base* TempItemBase = GetWorld()->SpawnActor<AItem_Base>(ItemPair.Value);
		if (IsValid(TempItemBase) && IsValid(TempItemBase->ItemObject))
		{
			TryAddItem(TempItemBase->ItemObject);
			TempItemBase->Destroy();
		}
	}
}
