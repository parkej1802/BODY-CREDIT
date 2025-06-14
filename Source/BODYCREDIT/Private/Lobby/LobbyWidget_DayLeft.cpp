// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_DayLeft.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Lobby/CLobbyWidget_Play.h"
#include "Session/NetGameInstance.h"
#include "Characters/CNox_Controller.h"
#include "Games/CMainGM.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CNox_Runner.h"

void ULobbyWidget_DayLeft::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_Confirm)
    {
        Button_Confirm->OnClicked.AddDynamic(this, &ThisClass::OnConfirmClicked);
        Button_Confirm->OnHovered.AddDynamic(this, &ThisClass::OnConfirmHovered);
        Button_Confirm->OnUnhovered.AddDynamic(this, &ThisClass::OnConfirmUnhovered);
    }

    Refresh();
}

void ULobbyWidget_DayLeft::OnConfirmClicked()
{
    GI = Cast<UNetGameInstance>(GetGameInstance());

    if (DayLeft && GI->SelectedPart != EPlayerPart::Basic)
    {
        if (ACNox_Controller* pc = Cast<ACNox_Controller>(GetWorld()->GetFirstPlayerController()))
        {
            ACMainGM *GameMode = Cast<ACMainGM>(GetWorld()->GetAuthGameMode());
            GameMode->GameTimer = GameMode->SetGameTimer;
            GameMode->bIsFailed = false;
            GameMode->IsStart = true;
            // GameMode->PlayGameStart();

            pc->SetInputMode(FInputModeGameOnly());
            pc->bShowMouseCursor = false;

            UGameplayStatics::SetGamePaused(GetWorld(), false);
            RemoveFromParent();


            APawn* Pawn = pc->GetPawn();

            ACNox_Runner* PlayerCharacter = Cast<ACNox_Runner>(Pawn);

            PlayerCharacter->ShowPlayerMainUI();


        }
        return;
    }

    if (LobbyPlayWidgetClass)
    {
        LobbyWidget_Play = CreateWidget<UCLobbyWidget_Play>(GetWorld(), LobbyPlayWidgetClass);
        if (LobbyWidget_Play)
        {
            LobbyWidget_Play->AddToViewport();
        }
    }
    RemoveFromParent();
}

void ULobbyWidget_DayLeft::OnConfirmHovered()
{
    if (Image_Button_Confirm_Hovered)
    {
        Image_Button_Confirm_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_DayLeft::OnConfirmUnhovered()
{
    if (Image_Button_Confirm_Hovered)
    {
        Image_Button_Confirm_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_DayLeft::Refresh()
{
    GI = Cast<UNetGameInstance>(GetGameInstance());

    DayLeft = GI->DayLeft;

    Text_DayLeft->SetText(FText::FromString(FString::FromInt(DayLeft)));
    Text_PayAmount->SetText(FText::FromString(FString::FromInt(GI->AmountToPay)));
}

