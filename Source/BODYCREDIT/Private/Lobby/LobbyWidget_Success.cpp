// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_Success.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"

void ULobbyWidget_Success::NativeConstruct()
{
	Super::NativeConstruct();

    if (Button_Continue)
    {
        Button_Continue->OnClicked.AddDynamic(this, &ULobbyWidget_Success::OnContinueClicked);
        Button_Continue->OnHovered.AddDynamic(this, &ULobbyWidget_Success::OnContinueHovered);
        Button_Continue->OnUnhovered.AddDynamic(this, &ULobbyWidget_Success::OnContinueUnhovered);
    }

    if (Anim_BackGround)
    {
        PlayAnimation(Anim_BackGround, 0.0f, 9999, EUMGSequencePlayMode::Forward);
    }
}

void ULobbyWidget_Success::OnContinueClicked()
{

}

void ULobbyWidget_Success::OnContinueHovered()
{
    if (Anim_Hovered_Continue)
    {
        PlayAnimation(Anim_Hovered_Continue);
    }

    if (Anim_Hovered_Continue_Loop)
    {
        PlayAnimation(Anim_Hovered_Continue_Loop, 0.0f, 9999, EUMGSequencePlayMode::Forward);
    }

    if (Image_Button_Continue_Hovered)
    {
        Image_Button_Continue_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_Success::OnContinueUnhovered()
{
    if (Anim_Unhovered_Continue)
    {
        PlayAnimation(Anim_Unhovered_Continue);
    }

    if (Anim_Hovered_Continue_Loop)
    {
        StopAnimation(Anim_Hovered_Continue_Loop);
    }

    if (Image_Button_Continue_Hovered)
    {
        Image_Button_Continue_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}
