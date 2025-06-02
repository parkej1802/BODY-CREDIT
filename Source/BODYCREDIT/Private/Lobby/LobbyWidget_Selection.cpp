// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_Selection.h"
#include "Components/Button.h"
#include "Lobby/LobbyWidget_Market.h"
#include "Lobby/LobbyWidget_WorkShop.h"
#include "Kismet/GameplayStatics.h"

void ULobbyWidget_Selection::NativeConstruct()
{
    Super::NativeConstruct();

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
}

void ULobbyWidget_Selection::OnPlayClicked()
{
    UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Levels/Untitled")));
    this->RemoveFromParent();
}

void ULobbyWidget_Selection::OnMarketClicked()
{
    if (LobbyMarketWidgetClass)
    {
        LobbyWidget_Market = CreateWidget<ULobbyWidget_Market>(GetWorld(), LobbyMarketWidgetClass);
        if (LobbyWidget_Market)
        {
            LobbyWidget_Market->AddToViewport();

            this->RemoveFromParent();
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

            this->RemoveFromParent();
        }
    }
}
