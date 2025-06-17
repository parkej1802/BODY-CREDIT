// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_InsufficientGold.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Session/NetGameInstance.h"

void ULobbyWidget_InsufficientGold::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_Confirm)
    {
        Button_Confirm->OnClicked.AddDynamic(this, &ULobbyWidget_InsufficientGold::OnConfirmClicked);
        Button_Confirm->OnHovered.AddDynamic(this, &ULobbyWidget_InsufficientGold::OnConfirmHovered);
        Button_Confirm->OnUnhovered.AddDynamic(this, &ULobbyWidget_InsufficientGold::OnConfirmUnhovered);
    }

    GI = Cast<UNetGameInstance>(GetGameInstance());
}

void ULobbyWidget_InsufficientGold::OnConfirmClicked()
{
    GI->PlayConfirmSound();
    RemoveFromParent();
}

void ULobbyWidget_InsufficientGold::OnConfirmHovered()
{
    //if (Anim_Hovered_Confirm)
    //{
    //    PlayAnimation(Anim_Hovered_Confirm);
    //}

    GI->PlayHoveredSound();

    if (Image_Button_Confirm_Hovered)
    {
        Image_Button_Confirm_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_InsufficientGold::OnConfirmUnhovered()
{
    if (Image_Button_Confirm_Hovered)
    {
        Image_Button_Confirm_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}