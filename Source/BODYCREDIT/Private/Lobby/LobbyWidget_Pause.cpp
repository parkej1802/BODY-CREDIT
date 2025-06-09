// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_Pause.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CNox_Controller.h"


void ULobbyWidget_Pause::NativeConstruct()
{
    Super::NativeConstruct();

    PC = Cast<ACNox_Controller>(GetOwningPlayer());

    APawn* Pawn = PC->GetPawn();

    PlayerCharacter = Cast<ACNox_Runner>(Pawn);

    if (Button_Continue)
    {
        Button_Continue->OnClicked.AddDynamic(this, &ULobbyWidget_Pause::OnContinueClicked);
        Button_Continue->OnHovered.AddDynamic(this, &ULobbyWidget_Pause::OnContinueHovered);
        Button_Continue->OnUnhovered.AddDynamic(this, &ULobbyWidget_Pause::OnContinueUnhovered);
    }

    if (Button_Exit)
    {
        Button_Exit->OnClicked.AddDynamic(this, &ULobbyWidget_Pause::OnExitClicked);
        Button_Exit->OnHovered.AddDynamic(this, &ULobbyWidget_Pause::OnExitHovered);
        Button_Exit->OnUnhovered.AddDynamic(this, &ULobbyWidget_Pause::OnExitUnhovered);
    }

    if (Anim_BackGround)
    {
        PlayAnimation(Anim_BackGround, 0.0f, 9999, EUMGSequencePlayMode::Forward);
    }
}

void ULobbyWidget_Pause::OnContinueClicked()
{
    PlayerCharacter->InventoryComp->PauseGame();
}

void ULobbyWidget_Pause::OnExitClicked()
{
    RemoveFromParent();
    PC->SetInputMode(FInputModeGameAndUI());
    PC->bShowMouseCursor = true;
}

void ULobbyWidget_Pause::OnContinueHovered()
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

void ULobbyWidget_Pause::OnContinueUnhovered()
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

void ULobbyWidget_Pause::OnExitHovered()
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

void ULobbyWidget_Pause::OnExitUnhovered()
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
