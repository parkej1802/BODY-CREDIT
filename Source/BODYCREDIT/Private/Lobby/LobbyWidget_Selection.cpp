// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_Selection.h"
#include "Components/Button.h"
#include "Lobby/CLobbyWidget_Play.h"
#include "Lobby/LobbyWidget_Market.h"
#include "Lobby/LobbyWidget_WorkShop.h"
#include "Kismet/GameplayStatics.h"
#include "Session/NetGameInstance.h"
#include "Inventory/AC_EquipComponent.h"
#include "Characters/CNox_Runner.h"
#include "Item/Lootable/Lootable_Box.h"
#include "AC_LootingInventoryComponent.h"
#include "EngineUtils.h"

void ULobbyWidget_Selection::NativeConstruct()
{
    Super::NativeConstruct();

    PC = GetOwningPlayer();
    FInputModeGameAndUI InputMode;
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = true;

    if (Button_Play)
    {
        Button_Play->OnClicked.AddDynamic(this, &ThisClass::OnPlayClicked);
    }

    if (Button_Market)
    {
        Button_Market->OnClicked.AddDynamic(this, &ThisClass::OnMarketClicked);
    }

    if (Button_WorkShop)
    {
        Button_WorkShop->OnClicked.AddDynamic(this, &ThisClass::OnWorkShopClicked);
    }

    APawn* Pawn = PC->GetPawn();

    PlayerCharacter = Cast<ACNox_Runner>(Pawn);
    // UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ULobbyWidget_Selection::OnPlayClicked()
{
    if (LobbyPlayWidgetClass)
    {
        LobbyWidget_Play = CreateWidget<UCLobbyWidget_Play>(GetWorld(), LobbyPlayWidgetClass);
        if (LobbyWidget_Play)
        {
            LobbyWidget_Play->AddToViewport();

            this->RemoveFromParent();
        }
    }

    UWorld* World = GetWorld();
    if (!World) return;

    for (TActorIterator<ALootable_Box> It(World); It; ++It)
    {
        ALootable_Box* LootableActor = *It;
        if (LootableActor && LootableActor->LootInventoryComp)
        {
            LootableActor->LootInventoryComp->RefreshInventory();
        }
    }

    FVector StartLocation(285.0f, 15.0f, -408.0f);
    PlayerCharacter->SetActorLocation(StartLocation);
    

    GI = Cast<UNetGameInstance>(GetGameInstance());
    if (GI) {
        GI->BeforePlayerGold = PlayerCharacter->EquipComp->CalculatePriceOfEquippedItem();
    }

    //// OpenLevel
    //UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Levels/Lab")));
    //this->RemoveFromParent();
    //PC = GetWorld()->GetFirstPlayerController();
    //PC->SetInputMode(FInputModeGameOnly());
    //PC->bShowMouseCursor = false;

}

void ULobbyWidget_Selection::OnMarketClicked()
{
    if (LobbyMarketWidgetClass)
    {
        LobbyWidget_Market = CreateWidget<ULobbyWidget_Market>(GetWorld(), LobbyMarketWidgetClass);
        if (LobbyWidget_Market)
        {
            LobbyWidget_Market->AddToViewport();

            RemoveFromParent();
        }
    }
}

void ULobbyWidget_Selection::OnWorkShopClicked()
{
    if (LobbyWorkShopWidgetClass)
    {
        LobbyWidget_WorkShop = CreateWidget<ULobbyWidget_WorkShop>(GetWorld(), LobbyWorkShopWidgetClass);
        if (LobbyWidget_WorkShop)
        {
            LobbyWidget_WorkShop->AddToViewport();

            RemoveFromParent();
        }
    }
}

