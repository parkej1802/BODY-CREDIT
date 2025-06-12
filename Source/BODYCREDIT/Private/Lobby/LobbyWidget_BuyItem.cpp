// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_BuyItem.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Lobby/LobbyWidget_Market.h"
#include "Components/TextBlock.h"
#include "Session/NetGameInstance.h"

void ULobbyWidget_BuyItem::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_Confirm)
    {
        Button_Confirm->OnClicked.AddDynamic(this, &ThisClass::OnConfirmClicked);
        Button_Confirm->OnHovered.AddDynamic(this, &ThisClass::OnConfirmHovered);
        Button_Confirm->OnUnhovered.AddDynamic(this, &ThisClass::OnConfirmUnhovered);
    }

    if (Button_Cancel)
    {
        Button_Cancel->OnClicked.AddDynamic(this, &ThisClass::OnCancelClicked);
        Button_Cancel->OnHovered.AddDynamic(this, &ThisClass::OnCancelHovered);
        Button_Cancel->OnUnhovered.AddDynamic(this, &ThisClass::OnCancelUnhovered);
    }

    SetUIGold();
}

void ULobbyWidget_BuyItem::OnConfirmClicked()
{
    HandleConfirmClicked();
}

void ULobbyWidget_BuyItem::OnCancelClicked()
{
    HandleCancelClicked();
}

void ULobbyWidget_BuyItem::OnConfirmHovered()
{
    if (Image_Button_Confirm_Hovered)
    {
        Image_Button_Confirm_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_BuyItem::OnConfirmUnhovered()
{
    if (Image_Button_Confirm_Hovered)
    {
        Image_Button_Confirm_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_BuyItem::OnCancelHovered()
{
    if (Image_Button_Cancel_Hovered)
    {
        Image_Button_Cancel_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_BuyItem::OnCancelUnhovered()
{
    if (Image_Button_Cancel_Hovered)
    {
        Image_Button_Cancel_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_BuyItem::HandleConfirmClicked()
{
    OnConfirmBuy.Broadcast(ItemObjctBuy);
    RemoveFromParent();
}

void ULobbyWidget_BuyItem::HandleCancelClicked()
{
    OnCancelBuy.Broadcast(ItemObjctBuy);
    RemoveFromParent();
}

void ULobbyWidget_BuyItem::SetItemToBuy(class UItemObject* ItemObject)
{
    ItemObjctBuy = ItemObject;
}

void ULobbyWidget_BuyItem::SetUIGold()
{
    float ItemPrice = ItemObjctBuy->ItemData.Price;
    float RemaingGold = MarketUI->GI->PlayerGold - ItemPrice;
    
    Text_ItemPrice->SetText(FText::AsNumber(ItemPrice));
    Text_RemaingGold->SetText(FText::AsNumber(RemaingGold));
}
