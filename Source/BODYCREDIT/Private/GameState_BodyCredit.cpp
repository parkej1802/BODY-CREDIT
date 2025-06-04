// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState_BodyCredit.h"
#include "Item/ItemObject.h"
#include "Item/Item_Base.h"
#include "Inventory/AC_InventoryBaseComponent.h"
#include "Inventory/Inventory_Tile.h"
#include "AC_LootingInventoryComponent.h"



void AGameState_BodyCredit::SpawnItemFromActor(UItemObject* ItemObject, AActor* Actor, bool GroundClamp)
{
	FVector SpawnLocation;

	SpawnLocation = Actor->GetActorLocation() + (Actor->GetActorForwardVector() * 150.f);

	if (GroundClamp)
	{
		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(Actor);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, SpawnLocation, (SpawnLocation - FVector(0.f, 0.f, 1000.f)), ECC_Visibility, TraceParams);

		if (bHit)
		{
			SpawnLocation = HitResult.Location;
		}
	}

	/*AItem_Base* SpawnedItem = GetWorld()->SpawnActor<AItem_Base>(ItemObject->GetItemClass(), SpawnLocation, FRotator::ZeroRotator);

	if (SpawnedItem)
	{
		SpawnedItem->ItemObject = ItemObject;
	}*/

	AItem_Base* ItemActor = Cast<AItem_Base>(ItemObject->ItemActorOwner.Get());

	if (ItemActor)
	{
		ItemActor->SetActorLocation(SpawnLocation);
		ItemActor->SetActorHiddenInGame(false);
		ItemActor->SetActorEnableCollision(true);
	}


}

void AGameState_BodyCredit::SpawnItemHiddenFromActor(class UItemObject* ItemObject, AActor* Actor, bool GroundClamp)
{
	FVector SpawnLocation;

	SpawnLocation = Actor->GetActorLocation() + (Actor->GetActorForwardVector() * 150.f);

	if (GroundClamp)
	{
		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(Actor);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, SpawnLocation, (SpawnLocation - FVector(0.f, 0.f, 1000.f)), ECC_Visibility, TraceParams);

		if (bHit)
		{
			SpawnLocation = HitResult.Location;
		}
	}

	AItem_Base* ItemActor = GetWorld()->SpawnActor<AItem_Base>(ItemObject->ItemClass, SpawnLocation, FRotator::ZeroRotator);
	ItemObject->ItemActorOwner = ItemActor;
	ItemActor->ItemObject = ItemObject;

	if (ItemActor)
	{
		ItemActor->SetActorLocation(SpawnLocation);
		ItemActor->SetActorHiddenInGame(true);
		ItemActor->SetActorEnableCollision(false);
	}

	if (ItemObject->ItemActorOwner.IsValid())
	{
		UAC_InventoryBaseComponent* InnerInventory = ItemObject->ItemActorOwner->LootInventoryComp;

		if (InnerInventory)
		{
			for (const FInventoryItemData& SubItemData : ItemObject->ContainedItems)
			{
				UItemObject* SubItem = NewObject<UItemObject>(InnerInventory);
				SubItem->ID = SubItemData.ID;
				SubItem->Dimensions = SubItemData.Dimensions;
				SubItem->Rotated = SubItemData.bRotated;
				SubItem->StartPosition = SubItemData.StartPosition;
				SubItem->ItemClass = SubItemData.ItemClass;
				SubItem->Icon = SubItemData.Icon;
				SubItem->RotatedIcon = SubItemData.RotatedIcon;
				SubItem->ItemType = SubItemData.ItemType;
				SubItem->CurrentIndex = SubItemData.CurrentIndex;

				AItem_Base* SubItemActor = GetWorld()->SpawnActor<AItem_Base>(
					SubItem->ItemClass,
					SpawnLocation,
					FRotator::ZeroRotator
				);

				if (SubItemActor)
				{
					SubItemActor->SetActorHiddenInGame(true);
					SubItemActor->SetActorEnableCollision(false);
					SubItemActor->ItemObject = SubItem;
					SubItem->ItemActorOwner = SubItemActor;
				}

				if (SubItem->IsRotated() != SubItemData.bRotated)
				{
					SubItem->Rotate();
				}

				FInventoryTile SubTile(SubItemData.StartPosition.X, SubItemData.StartPosition.Y);
				int32 SubIndex = InnerInventory->TileToIndex(SubTile);

				if (InnerInventory->IsRoomAvailable(SubItem, SubIndex))
				{
					InnerInventory->AddItemAt(SubItem, SubIndex);
				}
			}
		}
	}
}

void AGameState_BodyCredit::SpawnItemPlayerInventory(class UItemObject* ItemObject, AActor* Actor, bool GroundClamp)
{
	FVector SpawnLocation;

	SpawnLocation = Actor->GetActorLocation() + (Actor->GetActorForwardVector() * 150.f);

	if (GroundClamp)
	{
		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(Actor);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, SpawnLocation, (SpawnLocation - FVector(0.f, 0.f, 1000.f)), ECC_Visibility, TraceParams);

		if (bHit)
		{
			SpawnLocation = HitResult.Location;
		}
	}

	AItem_Base* ItemActor = GetWorld()->SpawnActor<AItem_Base>(ItemObject->ItemClass, SpawnLocation, FRotator::ZeroRotator);
	ItemObject->ItemActorOwner = ItemActor;
	ItemActor->ItemObject = ItemObject;

	if (ItemActor)
	{
		ItemActor->SetActorLocation(SpawnLocation);
		ItemActor->SetActorHiddenInGame(true);
		ItemActor->SetActorEnableCollision(false);
	}

	/*if (ItemObject->ItemActorOwner.IsValid())
	{
		UAC_InventoryBaseComponent* PlayerInventory = ;

		if (InnerInventory)
		{
			for (const FInventoryItemData& SubItemData : ItemObject->ContainedItems)
			{
				UItemObject* SubItem = NewObject<UItemObject>(InnerInventory);
				SubItem->ID = SubItemData.ID;
				SubItem->Dimensions = SubItemData.Dimensions;
				SubItem->Rotated = SubItemData.bRotated;
				SubItem->StartPosition = SubItemData.StartPosition;
				SubItem->ItemClass = SubItemData.ItemClass;
				SubItem->Icon = SubItemData.Icon;
				SubItem->RotatedIcon = SubItemData.RotatedIcon;
				SubItem->ItemType = SubItemData.ItemType;
				SubItem->CurrentIndex = SubItemData.CurrentIndex;

				AItem_Base* SubItemActor = GetWorld()->SpawnActor<AItem_Base>(
					SubItem->ItemClass,
					SpawnLocation,
					FRotator::ZeroRotator
				);

				if (SubItemActor)
				{
					SubItemActor->SetActorHiddenInGame(true);
					SubItemActor->SetActorEnableCollision(false);
					SubItemActor->ItemObject = SubItem;
					SubItem->ItemActorOwner = SubItemActor;
				}

				if (SubItem->IsRotated() != SubItemData.bRotated)
				{
					SubItem->Rotate();
				}

				FInventoryTile SubTile(SubItemData.StartPosition.X, SubItemData.StartPosition.Y);
				int32 SubIndex = InnerInventory->TileToIndex(SubTile);

				if (InnerInventory->IsRoomAvailable(SubItem, SubIndex))
				{
					InnerInventory->AddItemAt(SubItem, SubIndex);
				}
			}
		}
	}*/
}
