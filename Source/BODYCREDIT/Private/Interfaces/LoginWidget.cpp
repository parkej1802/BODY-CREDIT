// Fill out your copyright notice in the Description page of Project Settings.

#include "Interfaces/LoginWidget.h"
#include "Components/EditableText.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Session/NetGameInstance.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!GI || Edit_RoomName->GetText().IsEmpty()) return;

	GI->CreateMySession(Edit_RoomName->GetText().ToString(), Slider_PlayerCount->GetValue());

}

void ULoginWidget::CreateRoom()
{
	if (!GI || Edit_RoomName->GetText().IsEmpty()) return;

	GI->CreateMySession(Edit_RoomName->GetText().ToString(), Slider_PlayerCount->GetValue());

}

void ULoginWidget::OnValueChanged(float Value)
{
	Text_PlayerCount->SetText(FText::AsNumber(Value));

}
