// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_ItemDescription.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Item/ItemDT.h"
#include "Item/ItemObject.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Session/NetGameInstance.h"

void ULobbyWidget_ItemDescription::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_Confirm)
    {
        Button_Confirm->OnClicked.AddDynamic(this, &ThisClass::OnConfirmClicked);
        Button_Confirm->OnHovered.AddDynamic(this, &ThisClass::OnConfirmHovered);
        Button_Confirm->OnUnhovered.AddDynamic(this, &ThisClass::OnConfirmUnhovered);
    }
	Refresh();

	GI = Cast<UNetGameInstance>(GetGameInstance());
}

void ULobbyWidget_ItemDescription::OnConfirmClicked()
{
	GI->PlayConfirmSound();

	RemoveFromParent();
}

void ULobbyWidget_ItemDescription::OnConfirmHovered()
{
	GI->PlayHoveredSound();

    if (Image_Button_Confirm_Hovered)
    {
        Image_Button_Confirm_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_ItemDescription::OnConfirmUnhovered()
{
    if (Image_Button_Confirm_Hovered)
    {
        Image_Button_Confirm_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_ItemDescription::Refresh()
{
	if (!VerticalBox_ItemStat) return;

	FSlateBrush NewBrush;
	NewBrush.SetResourceObject(ItemObject->ItemData.Thumbnail);
	Image_Item->SetBrush(NewBrush);

	VerticalBox_ItemStat->ClearChildren();

	const FItemStatIncrease& Stat = ItemObject->ItemData.StatIncrease;

	bool bHasStat = false;

	auto AddStatText = [&](const FString& StatName, float Value)
		{
			if (FMath::IsNearlyZero(Value)) return;

			bHasStat = true;

			UTextBlock* StatText = NewObject<UTextBlock>(this);

			FString Sign = Value > 0.f ? TEXT("+") : TEXT("");
			FString Content = FString::Printf(TEXT("%s%.1f %s"), *Sign, Value, *StatName);

			StatText->SetText(FText::FromString(Content));
			StatText->Font.Size = 12;
			VerticalBox_ItemStat->AddChild(StatText);
		};

	AddStatText(TEXT("Health"), Stat.Health);
	AddStatText(TEXT("Strength"), Stat.Strength);
	AddStatText(TEXT("Stamina"), Stat.Stamina);
	AddStatText(TEXT("Move Speed"), Stat.MoveSpeed);
	AddStatText(TEXT("Armor"), Stat.Armor);
	AddStatText(TEXT("Accuracy"), Stat.Accuracy);
	AddStatText(TEXT("Carry Weight"), Stat.CarryWeight);
	AddStatText(TEXT("Humanity"), Stat.Humanity);
}
