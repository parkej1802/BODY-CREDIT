// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState_BodyCredit.h"
#include "Item/ItemObject.h"
#include "Item/Item_Base.h"



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
