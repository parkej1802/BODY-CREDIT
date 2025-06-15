#include "Lobby/CLobbyWidget_Option.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "Sound/SoundClass.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyWidget_Main.h"
#include "Session/NetGameInstance.h"

void UCLobbyWidget_Option::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<UNetGameInstance>(GetGameInstance());
	if (!GameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get GameInstance in CLobbyWidget_Option"));
		return;
	}

	{
		Button_Back->OnClicked.AddDynamic(this, &UCLobbyWidget_Option::OnBackButtonCliced);
		LobbySoundSlider->OnMouseCaptureEnd.AddDynamic(this, &UCLobbyWidget_Option::OnLobbySoundCaptureEnd);
		LobbySoundSlider->OnValueChanged.AddDynamic(this, &UCLobbyWidget_Option::OnLobbySoundVolumeChange);
		InGameSlider->OnValueChanged.AddDynamic(this, &UCLobbyWidget_Option::OnInGameVolumeChange);
		ExtractSoundSlider->OnValueChanged.AddDynamic(this, &UCLobbyWidget_Option::OnExtractSoundVolumeChange);
	}

	PlayAnimation(StartOptionTextAnim);
	PlayAnimation(StartOptionAnim);
	PlayAnimation(StartBackAnim);
}

void UCLobbyWidget_Option::OnBackButtonCliced()
{
	if (LobbyMainWidgetClass)
	{
		LobbyMainUI = CreateWidget<ULobbyWidget_Main>(GetWorld(), LobbyMainWidgetClass);
		if (LobbyMainUI)
		{
			LobbyMainUI->AddToViewport();
			RemoveFromParent();
		}
	}	
}

void UCLobbyWidget_Option::OnLobbySoundVolumeChange(float Value)
{
	// 슬라이더 값이 변경될 때는 사운드를 조절하지 않음
}

void UCLobbyWidget_Option::OnLobbySoundCaptureEnd()
{
	if (GameInstance && LobbySoundSlider)
	{
		GameInstance->SetLobbySound(LobbySoundSlider->GetValue());
	}
}

void UCLobbyWidget_Option::OnInGameVolumeChange(float Value)
{
	if (GameInstance)
	{
		GameInstance->SetInGameSound(Value);
	}
}

void UCLobbyWidget_Option::OnExtractSoundVolumeChange(float Value)
{
	if (GameInstance)
	{
		GameInstance->SetExtractSound(Value);
	}
}
