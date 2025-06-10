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

	// RefreshInventory();

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

	TArray<FName> CommonItems;
	TArray<FName> RareItems;
	TArray<FName> EpicItems;

	for (const FName& RowName : RowNames)
	{
		FItemData* ItemData = GI->ItemDataTable->FindRow<FItemData>(RowName, TEXT("ClassifyRarity"));
		if (!ItemData) continue;

		switch (ItemData->Rarity)
		{
		case EItemRarity::Common: CommonItems.Add(RowName); break;
		case EItemRarity::Rare:   RareItems.Add(RowName);   break;
		case EItemRarity::Epic:   EpicItems.Add(RowName);   break;
		default: break;
		}
	}

	while (ItemsAdded < MaxItemsToAdd)
	{
		float Roll = FMath::FRand();

		TArray<FName>* SelectedArray = nullptr;

		if (Roll <= 0.1f && EpicItems.Num() > 0)
		{
			SelectedArray = &EpicItems;
		}
		else if (Roll <= 0.4f && RareItems.Num() > 0)
		{
			SelectedArray = &RareItems;
		}
		else if (CommonItems.Num() > 0)
		{
			SelectedArray = &CommonItems;
		}

		if (SelectedArray && SelectedArray->Num() > 0)
		{
			int32 Index = FMath::RandRange(0, SelectedArray->Num() - 1);
			FName ChosenRow = (*SelectedArray)[Index];

			FItemData* ItemData = GI->ItemDataTable->FindRow<FItemData>(ChosenRow, TEXT("LootSpawn"));
			if (!ItemData || !ItemData->ItemClass) continue;

			AItem_Base* TempItemBase = GetWorld()->SpawnActor<AItem_Base>(ItemData->ItemClass);
			if (IsValid(TempItemBase) && IsValid(TempItemBase->ItemObject))
			{
				if (TryAddItem(TempItemBase->ItemObject))
				{
					TempItemBase->SetActorHiddenInGame(true);
					TempItemBase->SetActorEnableCollision(false);
				}
				else
				{
					TempItemBase->Destroy();
				}
			}
			ItemsAdded++;
		}
		else
		{
			break;
		}
	}
}

void UAC_LootingInventoryComponent::RefreshInventory()
{
	Items.Empty();
	AddRandomItem();
}

