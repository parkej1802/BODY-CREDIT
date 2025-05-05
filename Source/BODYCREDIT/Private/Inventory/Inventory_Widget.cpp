// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory_Widget.h"
#include "Inventory/Inventory_GridWidget.h"
#include "Characters/CNox_Runner.h"


bool UInventory_Widget::Initialize()
{
    if (!Super::Initialize()) return false;
    
    APlayerController* PC = GetOwningPlayer();

    APawn* Pawn = PC->GetPawn();
    
    PlayerCharacter = Cast<ACNox_Runner>(Pawn);

    InventoryComp = PlayerCharacter->InventoryComp;

    InventoryGridWidget->InitInventory(InventoryComp, TileSize);

    return true;
}
