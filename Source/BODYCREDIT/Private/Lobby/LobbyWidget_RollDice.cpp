// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_RollDice.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Session/NetGameInstance.h"
#include "Lobby/LobbyWidget_DayLeft.h"
#include "Games/StageDT.h"

void ULobbyWidget_RollDice::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_RollDice)
	{
		Button_RollDice->OnClicked.AddDynamic(this, &ThisClass::OnRollDiceClicked);
		Button_RollDice->OnHovered.AddDynamic(this, &ThisClass::OnRollDiceHovered);
		Button_RollDice->OnUnhovered.AddDynamic(this, &ThisClass::OnRollDiceUnhovered);
	}
	GI = Cast<UNetGameInstance>(GetGameInstance());

}

void ULobbyWidget_RollDice::OnRollDiceClicked()
{
	
	RollDice();
	
	GI->PlayConfirmSound();

	if (LobbyDayLeftWidgetClass)
	{
		LobbyWidget_DayLeft = CreateWidget<ULobbyWidget_DayLeft>(GetWorld(), LobbyDayLeftWidgetClass);
		if (LobbyWidget_DayLeft)
		{
			LobbyWidget_DayLeft->Refresh();
			LobbyWidget_DayLeft->AddToViewport();

			RemoveFromParent();
		}
	}

}

void ULobbyWidget_RollDice::OnRollDiceHovered()
{

	GI->PlayHoveredSound();

	if (Image_Button_RollDice_Hovered)
	{
		Image_Button_RollDice_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyWidget_RollDice::OnRollDiceUnhovered()
{
	if (Image_Button_RollDice_Hovered)
	{
		Image_Button_RollDice_Hovered->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULobbyWidget_RollDice::RollDice()
{
	int32 DiceResult = UKismetMathLibrary::RandomIntegerInRange(1, 3);

	GI->DayLeft = DiceResult;

	FString RowKey = FString::FromInt(GI->Day); 

	FStageData* StageData = GI->StageDataTable->FindRow<FStageData>(FName(*RowKey), TEXT("StageData"));
	if (StageData)
	{
		GI->AmountToPay = StageData->DayRequireGold;
	}
}
