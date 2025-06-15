// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_EscapeTimer.h"
#include "Components/TextBlock.h"

void ULobbyWidget_EscapeTimer::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULobbyWidget_EscapeTimer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsVisible)
	{
		EscapeTimer -= InDeltaTime;
		int32 Minutes = FMath::FloorToInt(EscapeTimer / 60.f);
		float SecondsFloat = FMath::Fmod(EscapeTimer, 60.f);

		FString TimeText = FString::Printf(TEXT("%02d:%05.2f"), Minutes, SecondsFloat);

		Text_Timer->SetText(FText::FromString(TimeText));
	}
}