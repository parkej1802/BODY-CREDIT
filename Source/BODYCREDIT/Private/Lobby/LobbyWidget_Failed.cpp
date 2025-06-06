// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_Failed.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"

void ULobbyWidget_Failed::NativeConstruct()
{
     Super::NativeConstruct();

    if (Button_Continue)
    {
        Button_Continue->OnClicked.AddDynamic(this, &ULobbyWidget_Failed::OnContinueClicked);
        Button_Continue->OnHovered.AddDynamic(this, &ULobbyWidget_Failed::OnContinueHovered);
        Button_Continue->OnUnhovered.AddDynamic(this, &ULobbyWidget_Failed::OnContinueUnhovered);
    }

    if (Button_Exit)
    {
        Button_Exit->OnClicked.AddDynamic(this, &ULobbyWidget_Failed::OnExitClicked);
        Button_Exit->OnHovered.AddDynamic(this, &ULobbyWidget_Failed::OnExitHovered);
        Button_Exit->OnUnhovered.AddDynamic(this, &ULobbyWidget_Failed::OnExitUnhovered);
    }

    if (Anim_BackGround)
    {
        PlayAnimation(Anim_BackGround, 0.0f, 9999, EUMGSequencePlayMode::Forward);
    }
}

void ULobbyWidget_Failed::OnContinueClicked()
{

}

void ULobbyWidget_Failed::OnExitClicked()
{
	
}

void ULobbyWidget_Failed::OnContinueHovered()
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

void ULobbyWidget_Failed::OnContinueUnhovered()
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

void ULobbyWidget_Failed::OnExitHovered()
{
    if (Anim_Hovered_Exit)
    {
        PlayAnimation(Anim_Hovered_Exit);
    }

    if (Anim_Hovered_Exit_Loop)
    {
        PlayAnimation(Anim_Hovered_Exit_Loop, 0.0f, 9999, EUMGSequencePlayMode::Forward);
    }

    if (Image_Button_Exit_Hovered)
    {
        Image_Button_Exit_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_Failed::OnExitUnhovered()
{
    if (Anim_Unhovered_Exit)
    {
        PlayAnimation(Anim_Unhovered_Exit);
    }

    if (Anim_Hovered_Exit_Loop)
    {
        StopAnimation(Anim_Hovered_Exit_Loop);
    }

    if (Image_Button_Exit_Hovered)
    {
        Image_Button_Exit_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}
