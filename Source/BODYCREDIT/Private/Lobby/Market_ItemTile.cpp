// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/Market_ItemTile.h"
#include "Components/Button.h"
#include "GameState_BodyCredit.h"
#include "Item/Item_Base.h"
#include "Inventory/AC_InventoryBaseComponent.h"

void UMarket_ItemTile::NativeConstruct()
{
	Super::NativeConstruct();


    if (Button_BuyItem)
    {
        Button_BuyItem->OnClicked.AddDynamic(this, &ThisClass::OnBuyItemClicked);
    }
}

void UMarket_ItemTile::OnBuyItemClicked()
{
    UItemObject* NewItemObject = NewObject<UItemObject>();
    NewItemObject->ItemClass = ItemData.ItemClass;
    NewItemObject->Icon = ItemData.Icon;
    NewItemObject->RotatedIcon = ItemData.RotatedIcon;
    NewItemObject->Dimensions = ItemData.Dimensions;
    NewItemObject->Rotated = ItemData.Rotated;
    NewItemObject->ItemType = ItemData.ItemType;

    AGameState_BodyCredit* GameState = GetWorld()->GetGameState<AGameState_BodyCredit>();
    if (GameState)
    {
        GameState->SpawnItemHiddenFromActor(NewItemObject, GetOwningPlayerPawn(), false);
    }
}
