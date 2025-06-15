#include "Lobby/CLobbyWidget_Play.h"

#include "EngineUtils.h"
#include "Global.h"
#include "Components/Button.h"
#include "Characters/CNox_Controller.h"
#include "Characters/CNox_Runner.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Session/NetGameInstance.h"
#include "Components/Image.h"
#include "Games/CMainGM.h"
#include "GameFramework/PlayerStart.h"

void UCLobbyWidget_Play::NativeConstruct()
{
	Super::NativeConstruct();


	GI = Cast<UNetGameInstance>(GetGameInstance());

	GI->SaveImageHead = ImageHead;
	GI->SaveImageBody = ImageBody;
	GI->SaveImageArm = ImageArm;
	GI->SaveImageLeg = ImageLeg;
	
	if (Button_Continue) 
	{
		Button_Continue->OnClicked.AddDynamic(this, &ThisClass::OnContinueClicked);
		Button_Continue->OnHovered.AddDynamic(this, &ThisClass::OnContinueHovered);
		Button_Continue->OnUnhovered.AddDynamic(this, &ThisClass::OnContinueUnhovered);
	}

	if (Button_Head && GI->AlivePart[EPlayerPart::Head])
	{
		Button_Head->OnClicked.AddDynamic(this, &ThisClass::OnHeadClicked);
		Button_Head->OnHovered.AddDynamic(this, &ThisClass::OnHeadHovered);
		Button_Head->OnUnhovered.AddDynamic(this, &ThisClass::OnHeadUnhovered);
	}
	else
	{
		Button_Head->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	}

	if (Button_Body && GI->AlivePart[EPlayerPart::Body])
	{
		Button_Body->OnClicked.AddDynamic(this, &ThisClass::OnBodyClicked);
		Button_Body->OnHovered.AddDynamic(this, &ThisClass::OnBodyHovered);
		Button_Body->OnUnhovered.AddDynamic(this, &ThisClass::OnBodyUnhovered);
	}
	else
	{
		Button_Body->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	}

	if (Button_Arm && GI->AlivePart[EPlayerPart::Arm])
	{
		Button_Arm->OnClicked.AddDynamic(this, &ThisClass::OnArmClicked);
		Button_Arm->OnHovered.AddDynamic(this, &ThisClass::OnArmHovered);
		Button_Arm->OnUnhovered.AddDynamic(this, &ThisClass::OnArmUnhovered);
	}
	else
	{
		Button_Arm->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	}

	if (Button_Leg && GI->AlivePart[EPlayerPart::Leg])
	{
		Button_Leg->OnClicked.AddDynamic(this, &ThisClass::OnLegClicked);
		Button_Leg->OnHovered.AddDynamic(this, &ThisClass::OnLegHovered);
		Button_Leg->OnUnhovered.AddDynamic(this, &ThisClass::OnLegUnhovered);
	}
	else
	{
		Button_Leg->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	}
}

void UCLobbyWidget_Play::OnContinueClicked()
{
	if (GI->SelectedPart == EPlayerPart::Basic) return;

	if (ACNox_Controller* pc = Cast<ACNox_Controller>(GetWorld()->GetFirstPlayerController()))
	{
		GameMode = Cast<ACMainGM>(GetWorld()->GetAuthGameMode());
		GameMode->GameTimer = GameMode->SetGameTimer;
		GameMode->bIsFailed = false;
		GameMode->IsStart = true;
		GameMode->ExtractTimerTriggerStart = true;
		
		pc->SetInputMode(FInputModeGameOnly());
		pc->bShowMouseCursor = false;
		
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		RemoveFromParent();


		APawn* Pawn = pc->GetPawn();
		ACNox_Runner* PlayerCharacter = Cast<ACNox_Runner>(Pawn);
		PlayerCharacter->ShowPlayerMainUI();
		PlayerCharacter->OnMovement();

		// for (TActorIterator<ACNox_EBase> It(GetWorld()); It; ++It)
		// {
		// 	(*It)->DayStart();
		// }
	}
}

void UCLobbyWidget_Play::OnContinueHovered()
{
	if (Image_Button_Continue_Hovered)
	{
		Image_Button_Continue_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCLobbyWidget_Play::OnContinueUnhovered()
{
	if (Image_Button_Continue_Hovered)
	{
		Image_Button_Continue_Hovered->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCLobbyWidget_Play::OnHeadClicked()
{
	GI->SelectedPart = EPlayerPart::Head;

	if (PreviousImage == Image_Button_Head_Hovered) return;

	TurnOffPreviousImage();
	PreviousImage = Image_Button_Head_Hovered;
	TurnOnPreviousImage();

	if (Image_Button_Head_Hovered)
	{
		Image_Button_Head_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCLobbyWidget_Play::OnHeadHovered()
{
	if (PreviousImage == Image_Button_Head_Hovered) return;

	if (Image_Button_Head_Hovered)
	{
		Image_Button_Head_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCLobbyWidget_Play::OnHeadUnhovered()
{
	if (PreviousImage == Image_Button_Head_Hovered) return;

	if (Image_Button_Head_Hovered)
	{
		Image_Button_Head_Hovered->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCLobbyWidget_Play::OnBodyClicked()
{
	GI->SelectedPart = EPlayerPart::Body;

	if (PreviousImage == Image_Button_Body_Hovered) return;

	TurnOffPreviousImage();
	PreviousImage = Image_Button_Body_Hovered;
	TurnOnPreviousImage();

	if (Image_Button_Body_Hovered)
	{
		Image_Button_Body_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCLobbyWidget_Play::OnBodyHovered()
{
	if (PreviousImage == Image_Button_Body_Hovered) return;

	if (Image_Button_Body_Hovered)
	{
		Image_Button_Body_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCLobbyWidget_Play::OnBodyUnhovered()
{
	if (PreviousImage == Image_Button_Body_Hovered) return;

	if (Image_Button_Body_Hovered)
	{
		Image_Button_Body_Hovered->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCLobbyWidget_Play::OnArmClicked()
{
	GI->SelectedPart = EPlayerPart::Arm;

	if (PreviousImage == Image_Button_Arm_Hovered) return;

	TurnOffPreviousImage();
	PreviousImage = Image_Button_Arm_Hovered;
	TurnOnPreviousImage();


	if (Image_Button_Arm_Hovered)
	{
		Image_Button_Arm_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCLobbyWidget_Play::OnArmHovered()
{
	if (PreviousImage == Image_Button_Arm_Hovered) return;

	if (Image_Button_Arm_Hovered)
	{
		Image_Button_Arm_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCLobbyWidget_Play::OnArmUnhovered()
{
	if (PreviousImage == Image_Button_Arm_Hovered) return;

	if (Image_Button_Arm_Hovered)
	{
		Image_Button_Arm_Hovered->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCLobbyWidget_Play::OnLegClicked()
{
	GI->SelectedPart = EPlayerPart::Leg;

	if (PreviousImage == Image_Button_Leg_Hovered) return;

	TurnOffPreviousImage();
	PreviousImage = Image_Button_Leg_Hovered;
	TurnOnPreviousImage();

	if (Image_Button_Leg_Hovered)
	{
		Image_Button_Leg_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCLobbyWidget_Play::OnLegHovered()
{
	if (PreviousImage == Image_Button_Leg_Hovered) return;

	if (Image_Button_Leg_Hovered)
	{
		Image_Button_Leg_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void UCLobbyWidget_Play::OnLegUnhovered()
{
	if (PreviousImage == Image_Button_Leg_Hovered) return;

	if (Image_Button_Leg_Hovered)
	{
		Image_Button_Leg_Hovered->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCLobbyWidget_Play::TurnOffPreviousImage()
{
	if (PreviousImage)
	{
		PreviousImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCLobbyWidget_Play::TurnOnPreviousImage()
{
	if (PreviousImage)
	{
		PreviousImage->SetVisibility(ESlateVisibility::Visible);
	}
}
