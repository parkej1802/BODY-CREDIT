// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/EscapeSuccessTriggerBox.h"
#include "Components/BoxComponent.h"
#include "Characters/CNox_Runner.h"
#include "Lobby/LobbyWidget_Success.h"
#include "Characters/CNox_Controller.h"
#include "Lobby/LobbyWidget_EscapeTimer.h"
#include "Session/NetGameInstance.h"

// Sets default values
AEscapeSuccessTriggerBox::AEscapeSuccessTriggerBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEscapeSuccessTriggerBox::OnOverlapBegin);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AEscapeSuccessTriggerBox::OnOverlapEnd);

	bIsPlayerInside = false;

}

// Called when the game starts or when spawned
void AEscapeSuccessTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	GI = Cast<UNetGameInstance>(GetGameInstance());
}

// Called every frame
void AEscapeSuccessTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//void AEscapeSuccessTriggerBox::HandleEscape()
//{
//	if (bIsPlayerInside && CachedPlayer)
//	{
//		CachedPlayer->IsEscape = true;
//		if (SuccessUI)
//		{
//			SuccessUI->Refresh();
//			SuccessUI->AddToViewport();
//			return;
//		}
//
//		if (LobbySuccessWidgetClass)
//		{
//			SuccessUI = CreateWidget<ULobbyWidget_Success>(GetWorld(), LobbySuccessWidgetClass);
//			if (SuccessUI)
//			{
//				SuccessUI->AddToViewport();
//				CachedPlayer->IsEscape = false;
//			}
//		}
//	}
//}
//
//void AEscapeSuccessTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	ACNox_Runner* PlayerCharacter = Cast<ACNox_Runner>(OtherActor);
//	if (PlayerCharacter)
//	{
//		bIsPlayerInside = true;
//		CachedPlayer = PlayerCharacter;
//		GetWorldTimerManager().SetTimer(EscapeTimerHandle, this, &AEscapeSuccessTriggerBox::HandleEscape, EscapeTime, false);
//		
//		if (EscapeTimerUI)
//		{
//			EscapeTimerUI->AddToViewport();
//			EscapeTimerUI->EscapeTimer = EscapeTime;
//			EscapeTimerUI->IsVisible = true;
//
//			return;
//		}
//
//		if (EscapeTimerWidgetClass)
//		{
//			EscapeTimerUI = CreateWidget<ULobbyWidget_EscapeTimer>(GetWorld(), EscapeTimerWidgetClass);
//			if (EscapeTimerUI)
//			{
//				EscapeTimerUI->AddToViewport();
//				EscapeTimerUI->EscapeTimer = EscapeTime;
//				EscapeTimerUI->IsVisible = true;
//			}
//		}
//	}
//}
//
//void AEscapeSuccessTriggerBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	ACNox_Runner* PlayerCharacter = Cast<ACNox_Runner>(OtherActor);
//	if (PlayerCharacter)
//	{
//		bIsPlayerInside = false;
//		CachedPlayer = nullptr;
//		GetWorldTimerManager().ClearTimer(EscapeTimerHandle);
//
//		if (EscapeTimerUI)
//		{
//			EscapeTimerUI->RemoveFromParent();
//			EscapeTimerUI->IsVisible = false;
//
//			return;
//		}
//	}
//}


void AEscapeSuccessTriggerBox::HandleEscape()
{
	if (SuccessUI)
	{
		if (SuccessUI->IsInViewport())
		{
			SuccessUI->RemoveFromParent();
		}
		SuccessUI = nullptr;
	}

	if (bIsPlayerInside && CachedPlayer && !CachedPlayer->State->IsDeadMode() && !GI->IsPlayerDead)
	{
		CachedPlayer->IsEscape = true;

		if (SuccessUI)
		{
			if (!SuccessUI->IsInViewport())
			{
				SuccessUI->Refresh();
				SuccessUI->AddToViewport();
			}
			return;
		}

		if (LobbySuccessWidgetClass)
		{
			SuccessUI = CreateWidget<ULobbyWidget_Success>(GetWorld(), LobbySuccessWidgetClass);
			if (SuccessUI)
			{
				SuccessUI->AddToViewport();
			}
		}
	}
}

void AEscapeSuccessTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACNox_Runner* PlayerCharacter = Cast<ACNox_Runner>(OtherActor);
	if (PlayerCharacter && !bIsPlayerInside)
	{
		bIsPlayerInside = true;
		CachedPlayer = PlayerCharacter;

		if (!GetWorldTimerManager().IsTimerActive(EscapeTimerHandle))
		{
			GetWorldTimerManager().SetTimer(EscapeTimerHandle, this, &AEscapeSuccessTriggerBox::HandleEscape, EscapeTime, false);
		}


		if (EscapeTimerUI)
		{
			if (!EscapeTimerUI->IsInViewport())
			{
				EscapeTimerUI->EscapeTimer = EscapeTime;
				EscapeTimerUI->IsVisible = true;
				EscapeTimerUI->AddToViewport();
			}
			return;
		}

		if (EscapeTimerWidgetClass)
		{
			EscapeTimerUI = CreateWidget<ULobbyWidget_EscapeTimer>(GetWorld(), EscapeTimerWidgetClass);
			if (EscapeTimerUI)
			{
				EscapeTimerUI->EscapeTimer = EscapeTime;
				EscapeTimerUI->IsVisible = true;
				EscapeTimerUI->AddToViewport();
			}
		}
	}
}

void AEscapeSuccessTriggerBox::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACNox_Runner* PlayerCharacter = Cast<ACNox_Runner>(OtherActor);
	if (PlayerCharacter && bIsPlayerInside)
	{
		bIsPlayerInside = false;
		CachedPlayer = nullptr;

		GetWorldTimerManager().ClearTimer(EscapeTimerHandle);

		if (EscapeTimerUI && EscapeTimerUI->IsInViewport())
		{
			EscapeTimerUI->RemoveFromParent();
			EscapeTimerUI->IsVisible = false;
		}
	}
}
