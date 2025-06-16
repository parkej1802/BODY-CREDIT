// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState_BodyCredit.h"
#include "Item/ItemObject.h"
#include "Item/Item_Base.h"
#include "Inventory/AC_InventoryBaseComponent.h"
#include "Inventory/Inventory_Tile.h"
#include "AC_LootingInventoryComponent.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Kismet/GameplayStatics.h"



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
	ItemObject->ItemActorOwner = ItemActor;
	ItemActor->ItemObject = ItemObject;

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

	AItem_Base* ItemActor = GetWorld()->SpawnActor<AItem_Base>(ItemObject->ItemData.ItemClass, SpawnLocation, FRotator::ZeroRotator);
	ItemObject->SetIconImages();
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
				SubItem->ItemData.ID = SubItemData.ID;
				SubItem->ItemData.Dimensions = SubItemData.Dimensions;
				SubItem->ItemData.Rotated = SubItemData.bRotated;
				SubItem->ItemData.StartPosition = SubItemData.StartPosition;
				SubItem->ItemData.ItemClass = SubItemData.ItemClass;
				SubItem->ItemData.Icon = SubItemData.Icon;
				SubItem->ItemData.RotatedIcon = SubItemData.RotatedIcon;
				SubItem->ItemData.ItemType = SubItemData.ItemType;
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

bool AGameState_BodyCredit::SpawnItemPlayerInventory(class UItemObject* ItemObject, AActor* Actor, bool GroundClamp)
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

	AItem_Base* ItemActor = GetWorld()->SpawnActor<AItem_Base>(ItemObject->ItemData.ItemClass, SpawnLocation, FRotator::ZeroRotator);
	ItemObject->ItemActorOwner = ItemActor;
	ItemActor->ItemObject = ItemObject;

	if (ItemActor)
	{
		ItemActor->SetActorLocation(SpawnLocation);
		ItemActor->SetActorHiddenInGame(true);
		ItemActor->SetActorEnableCollision(false);
	}

	PlayerCharacter = Cast<ACNox_Runner>(Actor);
	PlayerInventory = PlayerCharacter->InventoryComp;

	if (PlayerInventory->TryAddItem(ItemObject)) {
		return true;
	}
	
	return false;
}
