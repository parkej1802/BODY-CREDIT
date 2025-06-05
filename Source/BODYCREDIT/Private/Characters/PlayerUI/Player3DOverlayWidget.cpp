// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerUI/Player3DOverlayWidget.h"
#include "Characters/CNox_Runner.h"

void UPlayer3DOverlayWidget::NativeConstruct()
{
    PC = GetOwningPlayer();

    APawn* Pawn = PC->GetPawn();

    PlayerCharacter = Cast<ACNox_Runner>(Pawn);
}

FReply UPlayer3DOverlayWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsDragging && PlayerCharacter && PlayerCharacter->CaptureRoot)
    {
        FVector2D Delta = InMouseEvent.GetCursorDelta();
        float RotationSpeed = 0.3f;

        FRotator CurrentRot = PlayerCharacter->CaptureRoot->GetComponentRotation();
        CurrentRot.Yaw += Delta.X * RotationSpeed;

        PlayerCharacter->CaptureRoot->SetWorldRotation(CurrentRot);
    }

    return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UPlayer3DOverlayWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDragging = true;
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UPlayer3DOverlayWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);
    bIsDragging = false;
}

FReply UPlayer3DOverlayWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        bIsDragging = false;
        return FReply::Handled();
    }

    return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}
