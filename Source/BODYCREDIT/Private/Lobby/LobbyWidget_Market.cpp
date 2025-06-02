// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_Market.h"
#include "Components/Button.h"
#include "Lobby/LobbyWidget_Selection.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Inventory/Inventory_GridWidget.h"

void ULobbyWidget_Market::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_Back)
    {
        Button_Back->OnClicked.AddDynamic(this, &ThisClass::OnBackClicked);
    }

    PC = GetOwningPlayer();

    APawn* Pawn = PC->GetPawn();

    PlayerCharacter = Cast<ACNox_Runner>(Pawn);

    InventoryComp = PlayerCharacter->InventoryComp;

    InventoryGridWidget->InitInventory(InventoryComp, InventoryComp->InventoryTileSize);
    InventoryGridWidget->GridID = 0;
    InventoryGridWidget->PlayerController = PC;
    InventoryGridWidget->OwningInventoryWidget = this;
}

void ULobbyWidget_Market::OnBackClicked()
{
    if (LobbySelectionWidgetClass)
    {
        LobbyWidget_Selection = CreateWidget<ULobbyWidget_Selection>(GetWorld(), LobbySelectionWidgetClass);
        if (LobbyWidget_Selection)
        {
            LobbyWidget_Selection->AddToViewport();

            this->RemoveFromParent();
        }
    }
}
