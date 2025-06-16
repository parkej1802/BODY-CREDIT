// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_Failed.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"
#include "Session/NetGameInstance.h"
#include "Characters/CNox_Controller.h"
#include "Games/CMainGM.h"
#include "Lobby/LobbyWidget_Selection.h"
#include "Lobby/LobbyWidget_GameOver.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_EquipComponent.h"

void ULobbyWidget_Failed::NativeConstruct()
{
     Super::NativeConstruct();

    if (Button_Continue)
    {
        Button_Continue->OnClicked.AddDynamic(this, &ULobbyWidget_Failed::OnContinueClicked);
        Button_Continue->OnHovered.AddDynamic(this, &ULobbyWidget_Failed::OnContinueHovered);
        Button_Continue->OnUnhovered.AddDynamic(this, &ULobbyWidget_Failed::OnContinueUnhovered);
    }

	/*if (Button_Exit)
	{
		Button_Exit->OnClicked.AddDynamic(this, &ULobbyWidget_Failed::OnExitClicked);
		Button_Exit->OnHovered.AddDynamic(this, &ULobbyWidget_Failed::OnExitHovered);
		Button_Exit->OnUnhovered.AddDynamic(this, &ULobbyWidget_Failed::OnExitUnhovered);
	}*/

    if (Anim_BackGround)
    {
        PlayAnimation(Anim_BackGround, 0.0f, 9999, EUMGSequencePlayMode::Forward);
    }

    Refresh();

     {
         ACMainGM* GM = Cast<ACMainGM>(GetWorld()->GetAuthGameMode());
         GM->DestroyEnemy();
     }
}

void ULobbyWidget_Failed::OnContinueClicked()
{
    GI->SelectedPart = EPlayerPart::Basic;

    if (GI->Failed)
    {
        GI->Failed = false;
        RemoveFromParent();

        if (ACNox_Runner* runner = Cast<ACNox_Runner>(GetOwningPlayer()))
            runner->Reset();

        return;
    }
    
    if (LobbyWidget_LobbyGameOver)
    {
        LobbyWidget_LobbyGameOver = nullptr;
    }

    if (GI->RemainingLife == 0)
    {
        if (LobbyGameOverWidgetClass)
        {
            LobbyWidget_LobbyGameOver = CreateWidget<ULobbyWidget_GameOver>(GetWorld(), LobbyGameOverWidgetClass);
            if (LobbyWidget_LobbyGameOver)
            {
                LobbyWidget_LobbyGameOver->AddToViewport();

                RemoveFromParent();

                if (ACNox_Runner* runner = Cast<ACNox_Runner>(GetOwningPlayer()))
                    runner->Reset();
            }
        }
        return;
    }

    if (LobbySelectionWidgetClass)
    {
        LobbyWidget_Selection = CreateWidget<ULobbyWidget_Selection>(GetWorld(), LobbySelectionWidgetClass);
        if (LobbyWidget_Selection)
        {
            LobbyWidget_Selection->AddToViewport();

            if (ACNox_Runner* runner = Cast<ACNox_Runner>(GetOwningPlayer()))
                runner->Reset();

            this->RemoveFromParent();
        }
    }
}

//void ULobbyWidget_Failed::OnExitClicked()
//{
//    if (LobbySelectionWidgetClass)
//    {
//        LobbyWidget_Selection = CreateWidget<ULobbyWidget_Selection>(GetWorld(), LobbySelectionWidgetClass);
//        if (LobbyWidget_Selection)
//        {
//            LobbyWidget_Selection->AddToViewport();
//
//            this->RemoveFromParent();
//        }
//    } 
//}

void ULobbyWidget_Failed::OnContinueHovered()
{
    if (Anim_Hovered_Continue)
    {
        PlayAnimation(Anim_Hovered_Continue);
    }

    if (Anim_Hovered_Continue_Loop)
    {
        PlayAnimation(Anim_Hovered_Continue_Loop, 0.0f, 9999, EUMGSequencePlayMode::Forward);
    }

    if (Image_Button_Continue_Hovered)
    {
        Image_Button_Continue_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_Failed::OnContinueUnhovered()
{
    if (Anim_Unhovered_Continue)
    {
        PlayAnimation(Anim_Unhovered_Continue);
    }

    if (Anim_Hovered_Continue_Loop)
    {
        StopAnimation(Anim_Hovered_Continue_Loop);
    }

    if (Image_Button_Continue_Hovered)
    {
        Image_Button_Continue_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

//void ULobbyWidget_Failed::OnExitHovered()
//{
//    if (Anim_Hovered_Exit)
//    {
//        PlayAnimation(Anim_Hovered_Exit);
//    }
//
//    if (Anim_Hovered_Exit_Loop)
//    {
//        PlayAnimation(Anim_Hovered_Exit_Loop, 0.0f, 9999, EUMGSequencePlayMode::Forward);
//    }
//
//    if (Image_Button_Exit_Hovered)
//    {
//        Image_Button_Exit_Hovered->SetVisibility(ESlateVisibility::Visible);
//    }
//}
//
//void ULobbyWidget_Failed::OnExitUnhovered()
//{
//    if (Anim_Unhovered_Exit)
//    {
//        PlayAnimation(Anim_Unhovered_Exit);
//    }
//
//    if (Anim_Hovered_Exit_Loop)
//    {
//        StopAnimation(Anim_Hovered_Exit_Loop);
//    }
//
//    if (Image_Button_Exit_Hovered)
//    {
//        Image_Button_Exit_Hovered->SetVisibility(ESlateVisibility::Hidden);
//    }
//}

void ULobbyWidget_Failed::Refresh()
{
    PC = Cast<ACNox_Controller>(GetOwningPlayer());
    FInputModeGameAndUI InputMode;
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = true;

    GI = Cast<UNetGameInstance>(GetGameInstance());

    switch (GI->SelectedPart)
    {
    case EPlayerPart::Head:
        if (GI->SaveImageHead)
            Image_LostPart->SetBrush(GI->SaveImageHead->GetBrush());
        break;
    case EPlayerPart::Body:
        if (GI->SaveImageBody)
            Image_LostPart->SetBrush(GI->SaveImageBody->GetBrush());
        break;
    case EPlayerPart::Arm:
        if (GI->SaveImageArm)
            Image_LostPart->SetBrush(GI->SaveImageArm->GetBrush());
        break;
    case EPlayerPart::Leg:
        if (GI->SaveImageLeg)
            Image_LostPart->SetBrush(GI->SaveImageLeg->GetBrush());
        break;
    default:
        break;
    }

    if (GI->AlivePart.Contains(GI->SelectedPart))
    {
        GI->AlivePart[GI->SelectedPart] = false;
        GI->SelectedPart = EPlayerPart::Basic;
        --GI->RemainingLife;
        //GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Part Left %d"), RemainingLife));
    }

    Cast<ACNox_Runner>(PC->GetPawn())->EquipComp->EquippedItems.Empty();
    Cast<ACNox_Runner>(PC->GetPawn())->Reset();
}
