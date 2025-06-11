#include "Lobby/CLobbyWidget_Play.h"
#include "Global.h"
#include "Components/Button.h"
#include "Characters/CNox_Controller.h"
#include "Characters/CNox_Runner.h"
#include "Session/NetGameInstance.h"
#include "Components/Image.h"
#include "Games/CMainGM.h"

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
		Button_Continue->OnClicked.AddDynamic(this, &UCLobbyWidget_Play::OnContinueClicked);
	}

	if (Button_Head && GI->AlivePart[EPlayerPart::Head])
	{
		Button_Head->OnClicked.AddDynamic(this, &UCLobbyWidget_Play::OnHeadClicked);
	}
	else
	{
		Button_Head->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	}

	if (Button_Body && GI->AlivePart[EPlayerPart::Body])
	{
		Button_Body->OnClicked.AddDynamic(this, &UCLobbyWidget_Play::OnBodyClicked);
	}
	else
	{
		Button_Body->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	}

	if (Button_Arm && GI->AlivePart[EPlayerPart::Arm])
	{
		Button_Arm->OnClicked.AddDynamic(this, &UCLobbyWidget_Play::OnArmClicked);
	}
	else
	{
		Button_Arm->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	}

	if (Button_Leg && GI->AlivePart[EPlayerPart::Leg])
	{
		Button_Leg->OnClicked.AddDynamic(this, &UCLobbyWidget_Play::OnLegClicked);
	}
	else
	{
		Button_Leg->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
	}
}

void UCLobbyWidget_Play::OnContinueClicked()
{
    // OpenLevel
	if (ACNox_Controller* pc = Cast<ACNox_Controller>(GetWorld()->GetFirstPlayerController()))
	{
		GameMode = Cast<ACMainGM>(GetWorld()->GetAuthGameMode());
		GameMode->GameTimer = 10.f;
		GameMode->bIsFailed = false;
		GameMode->PlayGameStart();
		
		pc->SetInputMode(FInputModeGameOnly());
		pc->bShowMouseCursor = false;

		/*this->RemoveFromParent();

		UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Levels/Lab")));*/

		UGameplayStatics::SetGamePaused(GetWorld(), false);
		RemoveFromParent();

		APawn* Pawn = pc->GetPawn();

		ACNox_Runner* PlayerCharacter = Cast<ACNox_Runner>(Pawn);

		PlayerCharacter->ShowPlayerMainUI();


	}
}

void UCLobbyWidget_Play::OnHeadClicked()
{
	GI->SelectedPart = EPlayerPart::Head;
}

void UCLobbyWidget_Play::OnBodyClicked()
{
	GI->SelectedPart = EPlayerPart::Body;
}

void UCLobbyWidget_Play::OnArmClicked()
{
	GI->SelectedPart = EPlayerPart::Arm;
}

void UCLobbyWidget_Play::OnLegClicked()
{
	GI->SelectedPart = EPlayerPart::Leg;
}
