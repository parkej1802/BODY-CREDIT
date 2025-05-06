// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory_Widget.h"
#include "Inventory/Inventory_GridWidget.h"
#include "Characters/CNox_Runner.h"

void UInventory_Widget::NativeConstruct()
{
    Super::NativeConstruct();

    APlayerController* PC = GetOwningPlayer();

    APawn* Pawn = PC->GetPawn();

    PlayerCharacter = Cast<ACNox_Runner>(Pawn);

    InventoryComp = PlayerCharacter->InventoryComp;

    InventoryGridWidget->InitInventory(InventoryComp, TileSize);
}
