#include "Lobby/CLobbyWidget_Play.h"
#include "Global.h"
#include "Components/Button.h"
#include "Characters/CNox_Controller.h"

void UCLobbyWidget_Play::NativeConstruct()
{
	Super::NativeConstruct();

	Button_Next->OnClicked.AddDynamic(this, &UCLobbyWidget_Play::OnPlayClicked);

}

void UCLobbyWidget_Play::OnPlayClicked()
{
    // OpenLevel
	if (ACNox_Controller* pc = Cast<ACNox_Controller>(GetWorld()->GetFirstPlayerController()))
	{
		pc->SetInputMode(FInputModeGameOnly());
		pc->bShowMouseCursor = false;

		/*this->RemoveFromParent();

		UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Levels/Lab")));*/
	}
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	RemoveFromParent();

}
