// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_NoSpace.h"
#include "Components/Button.h"
#include "Components/Image.h"

void ULobbyWidget_NoSpace::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_Confirm)
    {
        Button_Confirm->OnClicked.AddDynamic(this, &ThisClass::OnConfirmClicked);
        Button_Confirm->OnHovered.AddDynamic(this, &ThisClass::OnConfirmHovered);
        Button_Confirm->OnUnhovered.AddDynamic(this, &ThisClass::OnConfirmUnhovered);
    }
}

void ULobbyWidget_NoSpace::OnConfirmClicked()
{
    RemoveFromParent();
}

void ULobbyWidget_NoSpace::OnConfirmHovered()
{
    if (Image_Button_Confirm_Hovered)
    {
        Image_Button_Confirm_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_NoSpace::OnConfirmUnhovered()
{
    if (Image_Button_Confirm_Hovered)
    {
        Image_Button_Confirm_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}
