// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/AC_MarketComponent.h"
#include "Engine/DataTable.h"

// Sets default values for this component's properties
UAC_MarketComponent::UAC_MarketComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_MarketComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Item/DT_ItemData.DT_ItemData"));
	
	
}


// Called every frame
void UAC_MarketComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FItemData> UAC_MarketComponent::GetMarketItems(EPlayerPart ItemType)
{
	TArray<FItemData> Weapons;

	if (!ItemDataTable)
		return Weapons;

	TArray<FItemData*> AllItems;
	ItemDataTable->GetAllRows<FItemData>(TEXT("Weapon Load"), AllItems);

	for (FItemData* Item : AllItems)
	{
		if (Item && Item->ItemType == ItemType)
		{
			Weapons.Add(*Item);
		}
	}
	return Weapons;
}

