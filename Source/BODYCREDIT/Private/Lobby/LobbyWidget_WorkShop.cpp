// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_WorkShop.h"
#include "Components/Button.h"
#include "Lobby/LobbyWidget_Selection.h"

void ULobbyWidget_WorkShop::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_Back)
    {
        Button_Back->OnClicked.AddDynamic(this, &ThisClass::OnBackClicked);
    }

}

void ULobbyWidget_WorkShop::OnBackClicked()
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
