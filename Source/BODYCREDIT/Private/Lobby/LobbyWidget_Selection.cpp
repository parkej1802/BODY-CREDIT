// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_Selection.h"
#include "Components/Button.h"
#include "Lobby/CLobbyWidget_Play.h"
#include "Lobby/LobbyWidget_Market.h"
#include "Lobby/LobbyWidget_WorkShop.h"
#include "Kismet/GameplayStatics.h"
#include "Session/NetGameInstance.h"
#include "Inventory/AC_EquipComponent.h"
#include "Characters/CNox_Runner.h"
#include "Item/Lootable/Lootable_Box.h"
#include "AC_LootingInventoryComponent.h"
#include "EngineUtils.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CNoxHPComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Games/CMainGM.h"
#include "Lobby/LobbyWidget_RollDice.h"
#include "Lobby/LobbyWidget_Payment.h"
#include "Lobby/LobbyWidget_DayLeft.h"
#include "Item/Item_Base.h"


void ULobbyWidget_Selection::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_Play)
    {
        Button_Play->OnClicked.AddDynamic(this, &ThisClass::OnPlayClicked);
        Button_Play->OnHovered.AddDynamic(this, &ThisClass::OnPlayHovered);
        Button_Play->OnUnhovered.AddDynamic(this, &ThisClass::OnPlayUnhovered);
    }

    if (Button_Market)
    {
        Button_Market->OnClicked.AddDynamic(this, &ThisClass::OnMarketClicked);
        Button_Market->OnHovered.AddDynamic(this, &ThisClass::OnMarketHovered);
        Button_Market->OnUnhovered.AddDynamic(this, &ThisClass::OnMarketUnhovered);
    }

    if (Button_WorkShop)
    {
        Button_WorkShop->OnClicked.AddDynamic(this, &ThisClass::OnWorkShopClicked);
        Button_WorkShop->OnHovered.AddDynamic(this, &ThisClass::OnWorkShopHovered);
        Button_WorkShop->OnUnhovered.AddDynamic(this, &ThisClass::OnWorkShopUnhovered);
    }

   Refresh();

    APawn* Pawn = PC->GetPawn();

    PlayerCharacter = Cast<ACNox_Runner>(Pawn);
    // UGameplayStatics::SetGamePaused(GetWorld(), true);

    GI = Cast<UNetGameInstance>(GetGameInstance());
    FString DayString = FString::Printf(TEXT("%d"), GI->Day);
    Text_DayCount->SetText(FText::FromString(DayString));

    GI->SetActorInitLocation();

    PlayerStatChange();

    GM = Cast<ACMainGM>(GetWorld()->GetAuthGameMode());
    GM->IsStart = false;
    GM->SpawnEnemy();
    
    PlayerCharacter->RemovePlayerMainUI();    

}

void ULobbyWidget_Selection::OnPlayClicked()
{
    
	/*if (LobbyPlayWidgetClass)
	{
		LobbyWidget_Play = CreateWidget<UCLobbyWidget_Play>(GetWorld(), LobbyPlayWidgetClass);
		if (LobbyWidget_Play)
		{
			LobbyWidget_Play->AddToViewport();

			RemoveFromParent();
		}
	}*/

    if (LobbyWidget_DayLeft)
    {
        LobbyWidget_DayLeft = nullptr;
    }


	if (GM) GM->ChangePlayerStartLocation();
    SetInventoryItems();

    if (GI) {

        GI->BeforePlayerGold = PlayerCharacter->EquipComp->CalculatePriceOfEquippedItem();
        GI->Day = GI->Day + 1;
        GI->DayLeft = GI->DayLeft - 1;

        if (GI->DayLeft < 0)
        {
            if (LobbyRollDiceWidgetClass)
            {
                LobbyWidget_RollDice = CreateWidget<ULobbyWidget_RollDice>(GetWorld(), LobbyRollDiceWidgetClass);
                if (LobbyWidget_RollDice)
                {
                    LobbyWidget_RollDice->AddToViewport();

                    RemoveFromParent();
                }
            }
            GI->BeforePlayerGold = PlayerCharacter->EquipComp->CalculatePriceOfEquippedItem();
            GI->Day = GI->Day + 1;
            // SetPlayerStartLocation();
            return;
        }

        else if (GI->DayLeft && GI->SelectedPart != EPlayerPart::Basic)
        {
            if (LobbyDayLeftWidgetClass)
            {
                LobbyWidget_DayLeft = CreateWidget<ULobbyWidget_DayLeft>(GetWorld(), LobbyDayLeftWidgetClass);
                if (LobbyWidget_DayLeft)
				{
                    LobbyWidget_DayLeft->AddToViewport();

                    RemoveFromParent();
                }
            }
        }
        else if (GI->PayTime)
        {
            GI->PayTime = false;

            if (LobbyPaymentWidgetClass)
            {
                LobbyWidget_Payment = CreateWidget<ULobbyWidget_Payment>(GetWorld(), LobbyPaymentWidgetClass);
                if (LobbyWidget_Payment)
                {
                    LobbyWidget_Payment->AddToViewport();

                    RemoveFromParent();
                }
            }
        }
        else
        {
            if (LobbyRollDiceWidgetClass)
            {
                LobbyWidget_RollDice = CreateWidget<ULobbyWidget_RollDice>(GetWorld(), LobbyRollDiceWidgetClass);
                if (LobbyWidget_RollDice)
                {
                    LobbyWidget_RollDice->AddToViewport();

                    RemoveFromParent();
                }
            }
        }
       
    }

    // SetPlayerStartLocation();

    //// OpenLevel
    //UGameplayStatics::OpenLevel(this, FName(TEXT("/Game/Levels/Lab")));
    //this->RemoveFromParent();
    //PC = GetWorld()->GetFirstPlayerController();
    //PC->SetInputMode(FInputModeGameOnly());
    //PC->bShowMouseCursor = false;

    
}

void ULobbyWidget_Selection::OnMarketClicked()
{
    if (LobbyWidget_Market)
    {
        LobbyWidget_Market->Refresh();
        LobbyWidget_Market->AddToViewport();
        RemoveFromParent();
        return;
    }

    if (LobbyMarketWidgetClass)
    {
        LobbyWidget_Market = CreateWidget<ULobbyWidget_Market>(GetWorld(), LobbyMarketWidgetClass);
        if (LobbyWidget_Market)
        {
            LobbyWidget_Market->AddToViewport();

            RemoveFromParent();
        }
    }
}

void ULobbyWidget_Selection::OnWorkShopClicked()
{
    if (LobbyWidget_WorkShop)
    {
        LobbyWidget_WorkShop->Refresh();
        LobbyWidget_WorkShop->AddToViewport();
        RemoveFromParent();
        return;
    }
    if (LobbyWorkShopWidgetClass)
    {
        LobbyWidget_WorkShop = CreateWidget<ULobbyWidget_WorkShop>(GetWorld(), LobbyWorkShopWidgetClass);
        if (LobbyWidget_WorkShop)
        {
            LobbyWidget_WorkShop->AddToViewport();

            RemoveFromParent();
        }
    }
}

void ULobbyWidget_Selection::OnPlayHovered()
{
	if (Image_Button_Play_Hovered)
	{
		Image_Button_Play_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyWidget_Selection::OnPlayUnhovered()
{
    if (Image_Button_Play_Hovered)
    {
        Image_Button_Play_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_Selection::OnMarketHovered()
{
    if (Image_Button_Market_Hovered)
    {
        Image_Button_Market_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_Selection::OnMarketUnhovered()
{
    if (Image_Button_Market_Hovered)
    {
        Image_Button_Market_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_Selection::OnWorkShopHovered()
{
    if (Image_Button_WorkShop_Hovered)
    {
        Image_Button_WorkShop_Hovered->SetVisibility(ESlateVisibility::Visible);
    }
}

void ULobbyWidget_Selection::OnWorkShopUnhovered()
{
    if (Image_Button_WorkShop_Hovered)
    {
        Image_Button_WorkShop_Hovered->SetVisibility(ESlateVisibility::Hidden);
    }
}

void ULobbyWidget_Selection::PlayerStatChange()
{
    float Power = PlayerCharacter->HPComp->Strength;
    float Health = PlayerCharacter->HPComp->Health;
    float Defense = PlayerCharacter->HPComp->Defense;
    float MovementSpeed = PlayerCharacter->HPComp->MovementSpeed;
    float Weight = PlayerCharacter->HPComp->Weight;
    float Stamina = PlayerCharacter->HPComp->Stamina;
    float Humanity = PlayerCharacter->HPComp->Humanity;
    float Debt = GI->Debt;

    Text_Power->SetText(FText::AsNumber(Power));
    Text_Health->SetText(FText::AsNumber(Health));
    Text_Speed->SetText(FText::AsNumber(MovementSpeed));
    Text_Weight->SetText(FText::AsNumber(Weight));
    Text_Energy->SetText(FText::AsNumber(Stamina));
    Text_Humanity->SetText(FText::AsNumber(Humanity));
    Text_Debt->SetText(FText::AsNumber(Debt));
}

void ULobbyWidget_Selection::SetPlayerStartLocation()
{

    FVector StartLocation(285.0f, 15.0f, -408.0f);
    PlayerCharacter->SetActorLocation(StartLocation);
}

void ULobbyWidget_Selection::SetInventoryItems()
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (TActorIterator<ALootable_Box> It(World); It; ++It)
    {
        ALootable_Box* LootableActor = *It;
        if (LootableActor && LootableActor->LootInventoryComp)
        {
            LootableActor->LootInventoryComp->RefreshInventory();
        }
    }
}

void ULobbyWidget_Selection::Refresh()
{
    PC = GetOwningPlayer();
    FInputModeGameAndUI InputMode;
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = true;

    APawn* Pawn = PC->GetPawn();

    PlayerCharacter = Cast<ACNox_Runner>(Pawn);
    // UGameplayStatics::SetGamePaused(GetWorld(), true);

    GI = Cast<UNetGameInstance>(GetGameInstance());
    FString DayString = FString::Printf(TEXT("%d"), GI->Day);
    Text_DayCount->SetText(FText::FromString(DayString));

    GI->SetActorInitLocation();

    PlayerStatChange();

    Cast<ACMainGM>(GetWorld()->GetAuthGameMode())->IsStart = false;

    PlayerCharacter->RemovePlayerMainUI();

    UWorld* World = GetWorld();
    if (!World) return;

    for (TActorIterator<AItem_Base> It(World); It; ++It)
    {
        AItem_Base* Item = *It;
        if (!Item->IsHidden())
        {
            Item->ItemObject = nullptr;
            Item->Destroy();
        }
    }
}
