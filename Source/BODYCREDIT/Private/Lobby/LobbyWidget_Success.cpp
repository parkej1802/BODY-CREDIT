// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_Success.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "Lobby/LobbyWidget_Selection.h"
#include "Characters/CNox_Controller.h"
#include "Session/NetGameInstance.h"
#include "Item/ItemDT.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_EquipComponent.h"
#include "Components/TextBlock.h"
#include "Games/CMainGM.h"

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

    Refresh();
}

void ULobbyWidget_Success::OnContinueClicked()
{
    if (LobbyWidget_Selection)
    {
        LobbyWidget_Selection->AddToViewport();
        LobbyWidget_Selection->Refresh();
        RemoveFromParent();
        return;
    }

    if (LobbySelectionWidgetClass)
    {
        LobbyWidget_Selection = CreateWidget<ULobbyWidget_Selection>(GetWorld(), LobbySelectionWidgetClass);
        if (LobbyWidget_Selection)
        {
            LobbyWidget_Selection->AddToViewport();
            RemoveFromParent();
        }
    }
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

void ULobbyWidget_Success::Refresh()
{
    ACNox_Controller* PC = Cast<ACNox_Controller>(GetOwningPlayer());
    FInputModeGameAndUI InputMode;
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = true;

    UNetGameInstance* GI = Cast<UNetGameInstance>(GetGameInstance());
    // GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("DayLeft %d"), GI->DayLeft));
    if (GI->DayLeft == 1)
    {
        GI->PayTime = true;
    }

    if (GI)
    {
        GI->AfterPlayerGold = GI->PlayerCharacter->EquipComp->CalculatePriceOfEquippedItem();
        int32 TotalProfit = GI->AfterPlayerGold - GI->BeforePlayerGold;
        Text_Profit->SetText(FText::FromString(FString::FromInt(TotalProfit)));
    }
    
    Cast<ACMainGM>(GetWorld()->GetAuthGameMode())->IsStart = false;
}