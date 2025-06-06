// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_Main.h"
#include "Components/Button.h"
#include "Lobby/LobbyWidget_Selection.h"

void ULobbyWidget_Main::NativeConstruct()
{
    Super::NativeConstruct();


    PC = GetOwningPlayer();
    FInputModeGameAndUI InputMode;
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = true;

    if (Button_NewGame)
    {
        Button_NewGame->OnClicked.AddDynamic(this, &ThisClass::OnNewGameClicked);
    }

    if (Button_Setting)
    {
        Button_Setting->OnClicked.AddDynamic(this, &ThisClass::OnSettingClicked);
    }

    if (Button_Exit)
    {
        Button_Exit->OnClicked.AddDynamic(this, &ThisClass::OnExitClicked);
    }


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

void ULobbyWidget_Main::OnSettingClicked()
{
    
}

void ULobbyWidget_Main::OnExitClicked()
{

}
