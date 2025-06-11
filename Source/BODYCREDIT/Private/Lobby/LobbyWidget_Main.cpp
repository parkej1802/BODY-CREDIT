// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_Main.h"
#include "Components/Button.h"
#include "Lobby/LobbyWidget_Selection.h"
#include "Components/Image.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Session/NetGameInstance.h"

void ULobbyWidget_Main::NativeConstruct()
{
    Super::NativeConstruct();


    PC = GetOwningPlayer();
    FInputModeGameAndUI InputMode;
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = true;
    // UGameplayStatics::SetGamePaused(GetWorld(), true);

    if (Button_NewGame)
    {
        Button_NewGame->OnClicked.AddDynamic(this, &ThisClass::OnNewGameClicked);
        Button_NewGame->OnHovered.AddDynamic(this, &ThisClass::OnNewGameHovered);
        Button_NewGame->OnUnhovered.AddDynamic(this, &ThisClass::OnNewGameUnhovered);
    }

    if (Button_Setting)
    {
        Button_Setting->OnClicked.AddDynamic(this, &ThisClass::OnSettingClicked);
        Button_Setting->OnHovered.AddDynamic(this, &ThisClass::OnSettingHovered);
        Button_Setting->OnUnhovered.AddDynamic(this, &ThisClass::OnSettingUnhovered);
    }

    if (Button_Exit)
    {
        Button_Exit->OnClicked.AddDynamic(this, &ThisClass::OnExitClicked);
        Button_Exit->OnHovered.AddDynamic(this, &ThisClass::OnExitHovered);
        Button_Exit->OnUnhovered.AddDynamic(this, &ThisClass::OnExitUnhovered);
    }

    PlayAnimation(Anim_Start_NewGame);
    PlayAnimation(Anim_Start_Setting);
    PlayAnimation(Anim_Start_Exit);
    UNetGameInstance* GI = Cast<UNetGameInstance>(GetGameInstance());
    GI->SetActorInitLocation();
}

void ULobbyWidget_Main::OnNewGameClicked()
{
    if (LobbySelectionWidgetClass)
    {
        LobbyWidget_Selection = CreateWidget<ULobbyWidget_Selection>(GetWorld(), LobbySelectionWidgetClass);
        if (LobbyWidget_Selection)
        {
            LobbyWidget_Selection->AddToViewport();

            this->RemoveFromParent();
        }
    }
}

void ULobbyWidget_Main::OnNewGameHovered()
{
    if (Anim_Hovered_NewGame)
    {
        PlayAnimation(Anim_Hovered_NewGame);
    }

    if (Image_Button_NewGame_Hovered)
    {
        Image_Button_NewGame_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_Main::OnNewGameUnhovered()
{
    if (Anim_Unhovered_NewGame)
    {
        PlayAnimation(Anim_Unhovered_NewGame);
    }

    if (Image_Button_NewGame_Hovered)
    {
        Image_Button_NewGame_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_Main::OnSettingClicked()
{
    
}

void ULobbyWidget_Main::OnSettingHovered()
{
    if (Anim_Hovered_Setting)
    {
        PlayAnimation(Anim_Hovered_Setting);
    }

    if (Image_Button_Setting_Hovered)
    {
        Image_Button_Setting_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_Main::OnSettingUnhovered()
{
    if (Anim_Unhovered_Setting)
    {
        PlayAnimation(Anim_Unhovered_Setting);
    }

    if (Image_Button_Setting_Hovered)
    {
        Image_Button_Setting_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_Main::OnExitClicked()
{
    PC = GetWorld()->GetFirstPlayerController();
    UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
}

void ULobbyWidget_Main::OnExitHovered()
{
    if (Anim_Hovered_Exit)
    {
        PlayAnimation(Anim_Hovered_Exit);
    }

    if (Image_Button_Exit_Hovered)
    {
        Image_Button_Exit_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_Main::OnExitUnhovered()
{
    if (Anim_Unhovered_Exit)
    {
        PlayAnimation(Anim_Unhovered_Exit);
    }

    if (Image_Button_Exit_Hovered)
    {
        Image_Button_Exit_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}
