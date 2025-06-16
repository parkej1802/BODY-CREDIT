// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/Market_ItemTile.h"
#include "Components/Button.h"
#include "GameState_BodyCredit.h"
#include "Item/Item_Base.h"
#include "Inventory/AC_InventoryBaseComponent.h"
#include "Inventory/Inventory_EquipmentTile.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Characters/CNox_Controller.h"
#include "Components/TextBlock.h"
#include "Session/NetGameInstance.h"
#include "Lobby/LobbyWidget_InsufficientGold.h"
#include "Lobby/LobbyWidget_Market.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"


void UMarket_ItemTile::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_BuyItem)
	{
		Button_BuyItem->OnClicked.AddDynamic(this, &ThisClass::OnBuyItemClicked);
		Button_BuyItem->OnHovered.AddDynamic(this, &ThisClass::OnBuyItemHovered);
		Button_BuyItem->OnUnhovered.AddDynamic(this, &ThisClass::OnBuyItemUnhovered);
	}

	PC = Cast<ACNox_Controller>(GetOwningPlayer());
	PlayerCharacter = Cast<ACNox_Runner>(PC->GetPawn());
	InventoryComp = PlayerCharacter->InventoryComp;

	Refresh();

}

void UMarket_ItemTile::OnBuyItemClicked()
{
	if (ItemData.Price > GI->PlayerGold)
	{
		if (InsufficientFundWidget)
		{
			InsufficientFundUI = CreateWidget<ULobbyWidget_InsufficientGold>(this, InsufficientFundWidget);
		}
		if (InsufficientFundUI)
		{
			InsufficientFundUI->AddToViewport();
		}
		return;
	}

	if (OwningMarket)
	{
		UItemObject* TempItemObject = NewObject<UItemObject>();
		TempItemObject->ItemData = ItemData;

		OwningMarket->ShowBuyUI(TempItemObject);
	}
}

void UMarket_ItemTile::OnBuyItemHovered()
{
	if (Image_Hovered)
	{
		Image_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMarket_ItemTile::OnBuyItemUnhovered()
{
	if (Image_Hovered)
	{
		Image_Hovered->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool UMarket_ItemTile::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UItemObject* ItemObject = Cast<UItemObject>(InOperation->Payload);

	if (UInventory_ItemWidget* DraggedWidget = Cast<UInventory_ItemWidget>(InOperation->DefaultDragVisual))
	{
		DraggedWidget->IsMoving = false;
	}

	if (UInventory_EquipmentTile* InventoryItemTileUI = Cast<UInventory_EquipmentTile>(InOperation->DefaultDragVisual)) {
		InventoryItemTileUI->IsMoving = false;

	}

	InventoryComp->TryAddItem(ItemObject);

	return true;
}

void UMarket_ItemTile::SetItemDescription()
{
	if (!VerticalBox_ItemData) return;

	VerticalBox_ItemData->ClearChildren();

	const FItemStatIncrease& Stat = ItemData.StatIncrease;

	bool bHasStat = false;

	auto AddStatText = [&](const FString& StatName, float Value)
		{
			if (FMath::IsNearlyZero(Value)) return;

			bHasStat = true;

			UTextBlock* StatText = NewObject<UTextBlock>(this);

			FString Sign = Value > 0.f ? TEXT("+") : TEXT("");
			FString Content = FString::Printf(TEXT("%s%.1f %s"), *Sign, Value, *StatName);

			StatText->SetText(FText::FromString(Content));
			StatText->Font.Size = 12;
			VerticalBox_ItemData->AddChild(StatText);
		};

	AddStatText(TEXT("Health"), Stat.Health);
	AddStatText(TEXT("Strength"), Stat.Strength);
	AddStatText(TEXT("Stamina"), Stat.Stamina);
	AddStatText(TEXT("Move Speed"), Stat.MoveSpeed);
	AddStatText(TEXT("Armor"), Stat.Armor);
	AddStatText(TEXT("Accuracy"), Stat.Accuracy);
	AddStatText(TEXT("Carry Weight"), Stat.CarryWeight);
	AddStatText(TEXT("Humanity"), Stat.Humanity);

	if (!bHasStat && VerticalBox_ItemNamePrice)
	{
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(VerticalBox_ItemNamePrice->Slot))
		{
			FVector2D CurrentPos = CanvasSlot->GetPosition();
			CanvasSlot->SetPosition(FVector2D(125.f, 0));
			Text_Name->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 32));
			Text_Price->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 32));
			TextBlock_P->SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 32));
		}
	}
}

void UMarket_ItemTile::Refresh()
{

	Text_Name->SetText(FText::FromName(ItemData.ItemName));
	Text_Price->SetText(FText::AsNumber(ItemData.Price));

	GI = Cast<UNetGameInstance>(GetGameInstance());

	SetItemDescription();
}
