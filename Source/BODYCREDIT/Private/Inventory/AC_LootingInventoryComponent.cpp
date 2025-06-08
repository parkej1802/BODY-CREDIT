// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_LootingInventoryComponent.h"
#include "Session/NetGameInstance.h"
#include "Item/Item_Base.h"

// Sets default values for this component's properties
UAC_LootingInventoryComponent::UAC_LootingInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_LootingInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner() && GetOwner()->IsA(AItem_Base::StaticClass()))
	{
		return;
	}

	AddRandomItem();

	// PreAddItem();
}


// Called every frame
void UAC_LootingInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Blue, FString::Printf(TEXT("IsDirty : %d"), IsDirty));
}

void UAC_LootingInventoryComponent::AddRandomItem()
{
	const int32 MaxItemsToAdd = FMath::RandRange(0, 5);
	int32 ItemsAdded = 0;

	UNetGameInstance* GI = GetWorld()->GetGameInstance<UNetGameInstance>();
	if (!GI || !GI->ItemDataTable) return;

	TArray<FName> RowNames = GI->ItemDataTable->GetRowNames();

	for (const FName& RowName : RowNames)
	{
		if (ItemsAdded >= MaxItemsToAdd)
			break;

		FItemData* ItemData = GI->ItemDataTable->FindRow<FItemData>(RowName, TEXT("PreAddItem"));
		if (!ItemData || !ItemData->ItemClass) continue;

		float Chance = 0.f;
		switch (ItemData->Rarity)
		{
		case EItemRarity::Common: Chance = 0.8f; break;
		case EItemRarity::Rare:   Chance = 0.3f; break;
		case EItemRarity::Epic:   Chance = 0.1f; break;
		}

		if (FMath::FRand() <= Chance)
		{
			AItem_Base* TempItemBase = GetWorld()->SpawnActor<AItem_Base>(ItemData->ItemClass);
			if (IsValid(TempItemBase) && IsValid(TempItemBase->ItemObject))
			{
				if (TryAddItem(TempItemBase->ItemObject)) {
					TempItemBase->SetActorHiddenInGame(true);
					TempItemBase->SetActorEnableCollision(false);
				}
				else {
					TempItemBase->Destroy();
				}
				ItemsAdded++;
			}
		}
	}
}

