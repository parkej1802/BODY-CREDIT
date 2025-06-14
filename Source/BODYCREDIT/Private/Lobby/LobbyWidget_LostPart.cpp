// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_LostPart.h"
#include "Components/Button.h"
#include "Lobby/LobbyWidget_RollDice.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void ULobbyWidget_LostPart::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_Continue)
    {
        Button_Continue->OnClicked.AddDynamic(this, &ThisClass::OnContinueClicked);
        Button_Continue->OnHovered.AddDynamic(this, &ThisClass::OnContinueHovered);
        Button_Continue->OnUnhovered.AddDynamic(this, &ThisClass::OnContinueUnhovered);
    }
}

void ULobbyWidget_LostPart::OnContinueClicked()
{
	
}

void ULobbyWidget_LostPart::OnContinueHovered()
{

}

void ULobbyWidget_LostPart::OnContinueUnhovered()
{

}
