// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_ItemMenu.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Item/ItemObject.h"
#include "Item/Item_Base.h"
#include "Session/NetGameInstance.h"
#include "Lobby/LobbyWidget_Market.h"

void ULobbyWidget_ItemMenu::NativeConstruct()
{
	Super::NativeConstruct();

    if (Button_Use)
    {
        Button_Use->OnClicked.AddDynamic(this, &ThisClass::OnUseClicked);
        Button_Use->OnHovered.AddDynamic(this, &ThisClass::OnUseHovered);
        Button_Use->OnUnhovered.AddDynamic(this, &ThisClass::OnUseUnhovered);
    }

    if (Button_Sell)
    {
        Button_Sell->OnClicked.AddDynamic(this, &ThisClass::OnSellClicked);
        Button_Sell->OnHovered.AddDynamic(this, &ThisClass::OnSellHovered);
        Button_Sell->OnUnhovered.AddDynamic(this, &ThisClass::OnSellUnhovered);
    }

    if (Button_Description)
    {
        Button_Description->OnClicked.AddDynamic(this, &ThisClass::OnDescriptionClicked);
        Button_Description->OnHovered.AddDynamic(this, &ThisClass::OnDescriptionHovered);
        Button_Description->OnUnhovered.AddDynamic(this, &ThisClass::OnDescriptionUnhovered);
    }
    GI = Cast<UNetGameInstance>(GetGameInstance());
}

void ULobbyWidget_ItemMenu::OnUseClicked()
{
    if (ItemObject)
    {
        ItemObject->ItemActorOwner->UseItem();
    }
    RemoveFromParent();
}

void ULobbyWidget_ItemMenu::OnSellClicked()
{
    if (GI->MarketUI)
    {
        GI->MarketUI->ShowSellUI(ItemObject);
    }
    RemoveFromParent();
}

void ULobbyWidget_ItemMenu::OnDescriptionClicked()
{
    RemoveFromParent();
}

void ULobbyWidget_ItemMenu::OnUseHovered()
{
    if (Image_Button_Use_Hovered)
    {
        Image_Button_Use_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_ItemMenu::OnUseUnhovered()
{
    if (Image_Button_Use_Hovered)
    {
        Image_Button_Use_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_ItemMenu::OnSellHovered()
{
    if (Image_Button_Sell_Hovered)
    {
        Image_Button_Sell_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_ItemMenu::OnSellUnhovered()
{
    if (Image_Button_Sell_Hovered)
    {
        Image_Button_Sell_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_ItemMenu::OnDescriptionHovered()
{
    if (Image_Button_Description_Hovered)
    {
        Image_Button_Description_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_ItemMenu::OnDescriptionUnhovered()
{
    if (Image_Button_Description_Hovered)
    {
        Image_Button_Description_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}
