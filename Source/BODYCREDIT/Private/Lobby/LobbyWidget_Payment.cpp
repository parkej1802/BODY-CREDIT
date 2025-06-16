// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_Payment.h"
#include "Components/Button.h"
#include "Lobby/LobbyWidget_RollDice.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Session/NetGameInstance.h"
#include "Lobby/LobbyWidget_Failed.h"

void ULobbyWidget_Payment::NativeConstruct()
{
	Super::NativeConstruct();

    if (Button_Payment)
    {
        Button_Payment->OnClicked.AddDynamic(this, &ThisClass::OnPaymentClicked);
        Button_Payment->OnHovered.AddDynamic(this, &ThisClass::OnPaymentHovered);
        Button_Payment->OnUnhovered.AddDynamic(this, &ThisClass::OnPaymentUnhovered);
    }

    if (Button_Skip)
    {
        Button_Skip->OnClicked.AddDynamic(this, &ThisClass::OnSkipClicked);
        Button_Skip->OnHovered.AddDynamic(this, &ThisClass::OnSkipHovered);
        Button_Skip->OnUnhovered.AddDynamic(this, &ThisClass::OnSkipUnhovered);
    }

    Refresh();
}


void ULobbyWidget_Payment::OnPaymentClicked()
{
    if (GI)
    {
        GI->SelectedPart = EPlayerPart::Basic;

        if (LobbyWidget_RollDice)
        {
            LobbyWidget_RollDice = nullptr;
        }

        if (GI->AmountToPay > GI->PlayerGold)
        {
            return;
        }
        else
        {
            GI->Debt = GI->Debt - GI->AmountToPay;
            GI->PlayerGold = GI->PlayerGold - GI->AmountToPay;
            if (LobbyRollDiceWidgetClass)
            {
                LobbyWidget_RollDice = CreateWidget<ULobbyWidget_RollDice>(GetWorld(), LobbyRollDiceWidgetClass);
                if (LobbyWidget_RollDice)
                {
                    LobbyWidget_RollDice->AddToViewport();

                    RemoveFromParent();
                }
            }
        }
    }
}

void ULobbyWidget_Payment::OnSkipClicked()
{
    if (LobbyWidget_RollDice)
    {
        LobbyWidget_RollDice = nullptr;
    }

	if (LobbyRollDiceWidgetClass)
	{
		LobbyWidget_RollDice = CreateWidget<ULobbyWidget_RollDice>(GetWorld(), LobbyRollDiceWidgetClass);
		if (LobbyWidget_RollDice)
		{
			LobbyWidget_RollDice->AddToViewport();

			RemoveFromParent();
		}
	}

    if (FailedUI)
    {
        FailedUI = nullptr;
    }

    if (FailedWidgetClass)
    {
        FailedUI = CreateWidget<ULobbyWidget_Failed>(GetWorld(), FailedWidgetClass);
        if (FailedUI)
        {
            GI->Failed = true;
            FailedUI->AddToViewport();
        }
    }

}

void ULobbyWidget_Payment::OnPaymentHovered()
{
    if (Image_Button_Payment_Hovered)
    {
        Image_Button_Payment_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_Payment::OnPaymentUnhovered()
{
    if (Image_Button_Payment_Hovered)
    {
        Image_Button_Payment_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}


void ULobbyWidget_Payment::OnSkipHovered()
{
    if (Image_Button_Skip_Hovered)
    {
        Image_Button_Skip_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_Payment::OnSkipUnhovered()
{
    if (Image_Button_Skip_Hovered)
    {
        Image_Button_Skip_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_Payment::Refresh()
{
    GI = Cast<UNetGameInstance>(GetGameInstance());
    Text_PayAmount->SetText(FText::FromString(FString::FromInt(GI->AmountToPay)));
}

