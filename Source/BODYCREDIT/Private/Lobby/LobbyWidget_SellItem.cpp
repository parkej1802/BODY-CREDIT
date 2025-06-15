// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_SellItem.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Item/ItemObject.h"
#include "Lobby/LobbyWidget_Market.h"
#include "Components/TextBlock.h"

void ULobbyWidget_SellItem::NativeConstruct()
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

    Refresh();
}

void ULobbyWidget_SellItem::OnConfirmClicked()
{
    HandleConfirmClicked();
}

void ULobbyWidget_SellItem::OnCancelClicked()
{
    HandleCancelClicked();
}

void ULobbyWidget_SellItem::OnConfirmHovered()
{
    if (Image_Button_Confirm_Hovered)
    {
        Image_Button_Confirm_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_SellItem::OnConfirmUnhovered()
{
    if (Image_Button_Confirm_Hovered)
    {
        Image_Button_Confirm_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_SellItem::OnCancelHovered()
{
    if (Image_Button_Cancel_Hovered)
    {
        Image_Button_Cancel_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_SellItem::OnCancelUnhovered()
{
    if (Image_Button_Cancel_Hovered)
    {
        Image_Button_Cancel_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_SellItem::HandleConfirmClicked()
{
    OnConfirmSell.Broadcast(ItemObjectSell);
    //MarketUI->HandleSellConfirm(ItemObjctSell);
    RemoveFromParent();
}

void ULobbyWidget_SellItem::HandleCancelClicked()
{
    OnCancelSell.Broadcast(ItemObjectSell);
   // MarketUI->HandleSellCancel(ItemObjctSell);
    RemoveFromParent();
}

void ULobbyWidget_SellItem::SetItemToSell(class UItemObject* ItemObject)
{
    ItemObjectSell = ItemObject;
}

void ULobbyWidget_SellItem::Refresh()
{
    if (ItemObjectSell && Text_ItemSellPrice)
    {
        int32 SellPrice = ItemObjectSell->GetSellPrice();
        FString PriceString = FString::Printf(TEXT("%d"), SellPrice);
        Text_ItemSellPrice->SetText(FText::FromString(PriceString));
    }
}

