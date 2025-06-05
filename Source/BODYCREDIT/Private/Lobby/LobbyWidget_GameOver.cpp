// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_GameOver.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"

void ULobbyWidget_GameOver::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_NewGame)
    {
        Button_NewGame->OnClicked.AddDynamic(this, &ULobbyWidget_GameOver::OnNewGameClicked);
        Button_NewGame->OnHovered.AddDynamic(this, &ULobbyWidget_GameOver::OnNewGameHovered);
        Button_NewGame->OnUnhovered.AddDynamic(this, &ULobbyWidget_GameOver::OnNewGameUnhovered);
    }

    if (Button_Exit)
    {
        Button_Exit->OnClicked.AddDynamic(this, &ULobbyWidget_GameOver::OnExitClicked);
        Button_Exit->OnHovered.AddDynamic(this, &ULobbyWidget_GameOver::OnExitHovered);
        Button_Exit->OnUnhovered.AddDynamic(this, &ULobbyWidget_GameOver::OnExitUnhovered);
    }

    if (Anim_Background)
    {
        PlayAnimation(Anim_Background, 0.0f, 9999, EUMGSequencePlayMode::Forward);
    }

    if (Anim_GameOver) 
    {
        PlayAnimation(Anim_GameOver, 0.0f, 9999, EUMGSequencePlayMode::Forward);
    }

}

void ULobbyWidget_GameOver::OnNewGameClicked()
{

}


void ULobbyWidget_GameOver::OnExitClicked()
{

}

void ULobbyWidget_GameOver::OnNewGameHovered()
{
    if (Anim_Hovered_NewGame)
    {
        PlayAnimation(Anim_Hovered_NewGame);
    }

    if (Anim_Hovered_NewGame_Loop)
    {
        PlayAnimation(Anim_Hovered_NewGame_Loop, 0.0f, 9999, EUMGSequencePlayMode::Forward);
    }

    if (Image_Button_NewGame_Hovered)
    {
        Image_Button_NewGame_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_GameOver::OnNewGameUnhovered()
{
    if (Anim_Unhovered_NewGame)
    {
        PlayAnimation(Anim_Unhovered_NewGame);
    }

    if (Anim_Hovered_NewGame_Loop)
    {
        StopAnimation(Anim_Hovered_NewGame_Loop);
    }

    if (Image_Button_NewGame_Hovered)
    {
        Image_Button_NewGame_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_GameOver::OnExitHovered()
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

void ULobbyWidget_GameOver::OnExitUnhovered()
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
