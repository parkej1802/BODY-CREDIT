// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_ItemMenu2.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Item/ItemObject.h"
#include "Item/Item_Base.h"
#include "Session/NetGameInstance.h"
#include "Lobby/LobbyWidget_Market.h"
#include "Lobby/LobbyWidget_ItemDescription.h"
#include "Components/TextBlock.h"

void ULobbyWidget_ItemMenu2::NativeConstruct()
{
	Super::NativeConstruct();

    if (Button_Function)
    {
        Button_Function->OnClicked.AddDynamic(this, &ThisClass::OnFunctionClicked);
        Button_Function->OnHovered.AddDynamic(this, &ThisClass::OnFunctionHovered);
        Button_Function->OnUnhovered.AddDynamic(this, &ThisClass::OnFunctionUnhovered);
    }

    if (Button_Description)
    {
        Button_Description->OnClicked.AddDynamic(this, &ThisClass::OnDescriptionClicked);
        Button_Description->OnHovered.AddDynamic(this, &ThisClass::OnDescriptionHovered);
        Button_Description->OnUnhovered.AddDynamic(this, &ThisClass::OnDescriptionUnhovered);
    }

    GI = Cast<UNetGameInstance>(GetGameInstance());
}

void ULobbyWidget_ItemMenu2::OnFunctionClicked()
{
    if (ItemObject)
    {
        ItemObject->ItemActorOwner->UseItem();
    }

    RemoveFromParent();
    ResetInputMode();
}

void ULobbyWidget_ItemMenu2::OnDescriptionClicked()
{
    if (ItemDescriptionUI)
    {
        ItemDescriptionUI->ItemObject = ItemObject;
        ItemDescriptionUI->Refresh();
        ItemDescriptionUI->AddToViewport();
        RemoveFromParent();
        ResetInputMode();
        return;
    }

    if (ItemDescriptionWidgetClass)
    {
        ItemDescriptionUI = CreateWidget<ULobbyWidget_ItemDescription>(this, ItemDescriptionWidgetClass);
    }
    if (ItemDescriptionUI)
    {
        ItemDescriptionUI->ItemObject = ItemObject;
        ItemDescriptionUI->AddToViewport();
    }

    RemoveFromParent();
    ResetInputMode();
}

void ULobbyWidget_ItemMenu2::OnFunctionHovered()
{
    if (Image_Button_Function_Hovered)
    {
        Image_Button_Function_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_ItemMenu2::OnFunctionUnhovered()
{
    if (Image_Button_Function_Hovered)
    {
        Image_Button_Function_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_ItemMenu2::OnDescriptionHovered()
{
    if (Image_Button_Description_Hovered)
    {
        Image_Button_Description_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_ItemMenu2::OnDescriptionUnhovered()
{
    if (Image_Button_Description_Hovered)
    {
        Image_Button_Description_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_ItemMenu2::ResetInputMode()
{
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputMode.SetHideCursorDuringCapture(false);

        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = true;
    }
}

void ULobbyWidget_ItemMenu2::Refresh()
{
    
}

