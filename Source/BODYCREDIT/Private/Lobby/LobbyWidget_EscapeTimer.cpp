// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_EscapeTimer.h"
#include "Components/TextBlock.h"
#include "Characters/CNox_Runner.h"
#include "Session/NetGameInstance.h"

void ULobbyWidget_EscapeTimer::NativeConstruct()
{
	Super::NativeConstruct();

	PlayerCharacter = Cast<ACNox_Runner>(GetWorld()->GetFirstPlayerController()->GetPawn());

	GI = Cast<UNetGameInstance>(GetGameInstance());
}

void ULobbyWidget_EscapeTimer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (PlayerCharacter->State->IsDeadMode())
	{
		GI->IsPlayerDead = true;
		return;
	}

	if (IsVisible)
	{
		if (EscapeTimer > 0.f)
		{
			EscapeTimer -= InDeltaTime;
			EscapeTimer = FMath::Max(0.f, EscapeTimer);
			int32 Minutes = FMath::FloorToInt(EscapeTimer / 60.f);
			float SecondsFloat = FMath::Fmod(EscapeTimer, 60.f);

			FString TimeText = FString::Printf(TEXT("%02d:%05.2f"), Minutes, SecondsFloat);

			Text_Timer->SetText(FText::FromString(TimeText));
		}
	}
}