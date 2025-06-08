// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyWidget_Market.h"
#include "Components/Button.h"
#include "Lobby/LobbyWidget_Selection.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Inventory/Inventory_GridWidget.h"
#include "Inventory/AC_MarketComponent.h"
#include "Components/ScrollBox.h"
#include "Lobby/Market_ItemTile.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Inventory/Inventory_EquipmentTile.h"
#include "Characters/CNox_Controller.h"

void ULobbyWidget_Market::NativeConstruct()
{
    Super::NativeConstruct();

    PC = Cast<ACNox_Controller>(GetOwningPlayer());
    FInputModeGameAndUI InputMode;
    PC->SetInputMode(InputMode);
    PC->bShowMouseCursor = true;

    if (Button_Back)
    {
        Button_Back->OnClicked.AddDynamic(this, &ThisClass::OnBackClicked);
    }

    if (Button_SelectWeapon)
    {
        Button_SelectWeapon->OnClicked.AddDynamic(this, &ThisClass::OnSelectWeaponClicked);
    }

    if (Button_SelectHead)
    {
        Button_SelectHead->OnClicked.AddDynamic(this, &ThisClass::OnSelectHeadClicked);
    }

    if (Button_SelectBody)
    {
        Button_SelectBody->OnClicked.AddDynamic(this, &ThisClass::OnSelectBodyClicked);
    }

    if (Button_SelectArm)
    {
        Button_SelectArm->OnClicked.AddDynamic(this, &ThisClass::OnSelectArmClicked);
    }

    if (Button_SelectLeg)
    {
        Button_SelectLeg->OnClicked.AddDynamic(this, &ThisClass::OnSelectLegClicked);
    }

    if (Button_SelectUtility)
    {
        Button_SelectUtility->OnClicked.AddDynamic(this, &ThisClass::OnSelectUtilityClicked);
    }

    APawn* Pawn = PC->GetPawn();

    PlayerCharacter = Cast<ACNox_Runner>(Pawn);

    InventoryComp = PlayerCharacter->InventoryComp;

    InventoryGridWidget->InitInventory(InventoryComp, InventoryComp->InventoryTileSize);
    InventoryGridWidget->GridID = 0;
    InventoryGridWidget->PlayerController = PC;

    MarketComp = PlayerCharacter->MarketComp;
}

void ULobbyWidget_Market::OnBackClicked()
{
    if (LobbySelectionWidgetClass)
    {
        LobbyWidget_Selection = CreateWidget<ULobbyWidget_Selection>(GetWorld(), LobbySelectionWidgetClass);
        if (LobbyWidget_Selection)
        {
            LobbyWidget_Selection->AddToViewport();

            this->RemoveFromParent();
        }
    }
}

void ULobbyWidget_Market::OnSelectWeaponClicked()
{
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
	{
		return;
	}

	VerticalBox_MarketItem->ClearChildren();

	TArray<FItemData> Weapons = MarketComp->GetMarketItems(EPlayerPart::Weapon1);

	const int32 NumPerRow = 2;
	TArray<UHorizontalBox*> RowBoxes;

	const int32 NumRows = FMath::CeilToInt((float)Weapons.Num() / NumPerRow);
	for (int32 Row = 0; Row < NumRows; ++Row)
	{
		UHorizontalBox* NewRow = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());
		RowBoxes.Add(NewRow);
	}

	for (int32 i = 0; i < Weapons.Num(); ++i)
	{
		int32 RowIndex = i / NumPerRow;

		UMarket_ItemTile* ItemWidget = CreateWidget<UMarket_ItemTile>(this, MarketItemWidget);
		if (ItemWidget)
		{
            if (ItemWidget->Image_Item && Weapons[i].Thumbnail)
            {
                FSlateBrush NewBrush;
                NewBrush.SetResourceObject(Weapons[i].Thumbnail);
                NewBrush.ImageSize = ImageSize;
                ItemWidget->Image_Item->SetBrush(NewBrush);
                ItemWidget->ItemData = Weapons[i];
                ItemWidget->InventoryBaseComp = InventoryComp;
            }

			UHorizontalBox* TargetRow = RowBoxes[RowIndex];
			if (TargetRow)
			{
				UHorizontalBoxSlot* SlotH = TargetRow->AddChildToHorizontalBox(ItemWidget);
				if (Slot)
				{
					SlotH->SetHorizontalAlignment(HAlign_Center);
					SlotH->SetVerticalAlignment(VAlign_Center);
				}
			}
		}
	}

	for (UHorizontalBox* RowBox : RowBoxes)
	{
		if (RowBox)
		{
			UVerticalBoxSlot* VSlot = VerticalBox_MarketItem->AddChildToVerticalBox(RowBox);
			if (VSlot)
			{
				VSlot->SetHorizontalAlignment(HAlign_Fill);
			}
		}
	}
}

void ULobbyWidget_Market::OnSelectHeadClicked()
{
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
	{
		return;
	}

	VerticalBox_MarketItem->ClearChildren();

	TArray<FItemData> Weapons = MarketComp->GetMarketItems(EPlayerPart::Head);

	const int32 NumPerRow = 2;
	TArray<UHorizontalBox*> RowBoxes;

	const int32 NumRows = FMath::CeilToInt((float)Weapons.Num() / NumPerRow);
	for (int32 Row = 0; Row < NumRows; ++Row)
	{
		UHorizontalBox* NewRow = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());
		RowBoxes.Add(NewRow);
	}

	for (int32 i = 0; i < Weapons.Num(); ++i)
	{
		int32 RowIndex = i / NumPerRow;

		UMarket_ItemTile* ItemWidget = CreateWidget<UMarket_ItemTile>(this, MarketItemWidget);
		if (ItemWidget)
		{
			if (ItemWidget->Image_Item && Weapons[i].Thumbnail)
			{
				FSlateBrush NewBrush;
				NewBrush.SetResourceObject(Weapons[i].Thumbnail);
				NewBrush.ImageSize = ImageSize;
				ItemWidget->Image_Item->SetBrush(NewBrush);
				ItemWidget->ItemData = Weapons[i];
				ItemWidget->InventoryBaseComp = InventoryComp;
			}

			UHorizontalBox* TargetRow = RowBoxes[RowIndex];
			if (TargetRow)
			{
				UHorizontalBoxSlot* SlotH = TargetRow->AddChildToHorizontalBox(ItemWidget);
				if (Slot)
				{
					SlotH->SetHorizontalAlignment(HAlign_Center);
					SlotH->SetVerticalAlignment(VAlign_Center);
				}
			}
		}
	}

	for (UHorizontalBox* RowBox : RowBoxes)
	{
		if (RowBox)
		{
			UVerticalBoxSlot* VSlot = VerticalBox_MarketItem->AddChildToVerticalBox(RowBox);
			if (VSlot)
			{
				VSlot->SetHorizontalAlignment(HAlign_Fill);
			}
		}
	}
}

void ULobbyWidget_Market::OnSelectBodyClicked()
{
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
	{
		return;
	}

	VerticalBox_MarketItem->ClearChildren();

	TArray<FItemData> Weapons = MarketComp->GetMarketItems(EPlayerPart::Body);

	const int32 NumPerRow = 2;
	TArray<UHorizontalBox*> RowBoxes;

	const int32 NumRows = FMath::CeilToInt((float)Weapons.Num() / NumPerRow);
	for (int32 Row = 0; Row < NumRows; ++Row)
	{
		UHorizontalBox* NewRow = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());
		RowBoxes.Add(NewRow);
	}

	for (int32 i = 0; i < Weapons.Num(); ++i)
	{
		int32 RowIndex = i / NumPerRow;

		UMarket_ItemTile* ItemWidget = CreateWidget<UMarket_ItemTile>(this, MarketItemWidget);
		if (ItemWidget)
		{
			if (ItemWidget->Image_Item && Weapons[i].Thumbnail)
			{
				FSlateBrush NewBrush;
				NewBrush.SetResourceObject(Weapons[i].Thumbnail);
				NewBrush.ImageSize = ImageSize;
				ItemWidget->Image_Item->SetBrush(NewBrush);
				ItemWidget->ItemData = Weapons[i];
				ItemWidget->InventoryBaseComp = InventoryComp;
			}

			UHorizontalBox* TargetRow = RowBoxes[RowIndex];
			if (TargetRow)
			{
				UHorizontalBoxSlot* SlotH = TargetRow->AddChildToHorizontalBox(ItemWidget);
				if (Slot)
				{
					SlotH->SetHorizontalAlignment(HAlign_Center);
					SlotH->SetVerticalAlignment(VAlign_Center);
				}
			}
		}
	}

	for (UHorizontalBox* RowBox : RowBoxes)
	{
		if (RowBox)
		{
			UVerticalBoxSlot* VSlot = VerticalBox_MarketItem->AddChildToVerticalBox(RowBox);
			if (VSlot)
			{
				VSlot->SetHorizontalAlignment(HAlign_Fill);
			}
		}
	}
}

void ULobbyWidget_Market::OnSelectArmClicked()
{
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
	{
		return;
	}

	VerticalBox_MarketItem->ClearChildren();

	TArray<FItemData> Weapons = MarketComp->GetMarketItems(EPlayerPart::Arm);

	const int32 NumPerRow = 2;
	TArray<UHorizontalBox*> RowBoxes;

	const int32 NumRows = FMath::CeilToInt((float)Weapons.Num() / NumPerRow);
	for (int32 Row = 0; Row < NumRows; ++Row)
	{
		UHorizontalBox* NewRow = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());
		RowBoxes.Add(NewRow);
	}

	for (int32 i = 0; i < Weapons.Num(); ++i)
	{
		int32 RowIndex = i / NumPerRow;

		UMarket_ItemTile* ItemWidget = CreateWidget<UMarket_ItemTile>(this, MarketItemWidget);
		if (ItemWidget)
		{
			if (ItemWidget->Image_Item && Weapons[i].Thumbnail)
			{
				FSlateBrush NewBrush;
				NewBrush.SetResourceObject(Weapons[i].Thumbnail);
				NewBrush.ImageSize = ImageSize;
				ItemWidget->Image_Item->SetBrush(NewBrush);
				ItemWidget->ItemData = Weapons[i];
				ItemWidget->InventoryBaseComp = InventoryComp;
			}

			UHorizontalBox* TargetRow = RowBoxes[RowIndex];
			if (TargetRow)
			{
				UHorizontalBoxSlot* SlotH = TargetRow->AddChildToHorizontalBox(ItemWidget);
				if (Slot)
				{
					SlotH->SetHorizontalAlignment(HAlign_Center);
					SlotH->SetVerticalAlignment(VAlign_Center);
				}
			}
		}
	}

	for (UHorizontalBox* RowBox : RowBoxes)
	{
		if (RowBox)
		{
			UVerticalBoxSlot* VSlot = VerticalBox_MarketItem->AddChildToVerticalBox(RowBox);
			if (VSlot)
			{
				VSlot->SetHorizontalAlignment(HAlign_Fill);
			}
		}
	}
}

void ULobbyWidget_Market::OnSelectLegClicked()
{
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
	{
		return;
	}

	VerticalBox_MarketItem->ClearChildren();

	TArray<FItemData> Weapons = MarketComp->GetMarketItems(EPlayerPart::Leg);

	const int32 NumPerRow = 2;
	TArray<UHorizontalBox*> RowBoxes;

	const int32 NumRows = FMath::CeilToInt((float)Weapons.Num() / NumPerRow);
	for (int32 Row = 0; Row < NumRows; ++Row)
	{
		UHorizontalBox* NewRow = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());
		RowBoxes.Add(NewRow);
	}

	for (int32 i = 0; i < Weapons.Num(); ++i)
	{
		int32 RowIndex = i / NumPerRow;

		UMarket_ItemTile* ItemWidget = CreateWidget<UMarket_ItemTile>(this, MarketItemWidget);
		if (ItemWidget)
		{
			if (ItemWidget->Image_Item && Weapons[i].Thumbnail)
			{
				FSlateBrush NewBrush;
				NewBrush.SetResourceObject(Weapons[i].Thumbnail);
				NewBrush.ImageSize = ImageSize;
				ItemWidget->Image_Item->SetBrush(NewBrush);
				ItemWidget->ItemData = Weapons[i];
				ItemWidget->InventoryBaseComp = InventoryComp;

			}

			UHorizontalBox* TargetRow = RowBoxes[RowIndex];
			if (TargetRow)
			{
				UHorizontalBoxSlot* SlotH = TargetRow->AddChildToHorizontalBox(ItemWidget);
				if (Slot)
				{
					SlotH->SetHorizontalAlignment(HAlign_Center);
					SlotH->SetVerticalAlignment(VAlign_Center);
				}
			}
		}
	}

	for (UHorizontalBox* RowBox : RowBoxes)
	{
		if (RowBox)
		{
			UVerticalBoxSlot* VSlot = VerticalBox_MarketItem->AddChildToVerticalBox(RowBox);
			if (VSlot)
			{
				VSlot->SetHorizontalAlignment(HAlign_Fill);
			}
		}
	}
}

void ULobbyWidget_Market::OnSelectUtilityClicked()
{
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
	{
		return;
	}

	VerticalBox_MarketItem->ClearChildren();

	TArray<FItemData> Weapons = MarketComp->GetMarketItems(EPlayerPart::Backpack);
	TArray<FItemData> ChestRigs = MarketComp->GetMarketItems(EPlayerPart::ChestRigs);

	Weapons.Append(ChestRigs);

	const int32 NumPerRow = 2;
	TArray<UHorizontalBox*> RowBoxes;

	const int32 NumRows = FMath::CeilToInt((float)Weapons.Num() / NumPerRow);
	for (int32 Row = 0; Row < NumRows; ++Row)
	{
		UHorizontalBox* NewRow = NewObject<UHorizontalBox>(this, UHorizontalBox::StaticClass());
		RowBoxes.Add(NewRow);
	}

	for (int32 i = 0; i < Weapons.Num(); ++i)
	{
		int32 RowIndex = i / NumPerRow;

		UMarket_ItemTile* ItemWidget = CreateWidget<UMarket_ItemTile>(this, MarketItemWidget);
		if (ItemWidget)
		{
			if (ItemWidget->Image_Item && Weapons[i].Thumbnail)
			{
				FSlateBrush NewBrush;
				NewBrush.SetResourceObject(Weapons[i].Thumbnail);
				NewBrush.ImageSize = ImageSize;
				ItemWidget->Image_Item->SetBrush(NewBrush);
				ItemWidget->ItemData = Weapons[i];
				ItemWidget->InventoryBaseComp = InventoryComp;
			}

			UHorizontalBox* TargetRow = RowBoxes[RowIndex];
			if (TargetRow)
			{
				UHorizontalBoxSlot* SlotH = TargetRow->AddChildToHorizontalBox(ItemWidget);
				if (Slot)
				{
					SlotH->SetHorizontalAlignment(HAlign_Center);
					SlotH->SetVerticalAlignment(VAlign_Center);
				}
			}
		}
	}

	for (UHorizontalBox* RowBox : RowBoxes)
	{
		if (RowBox)
		{
			UVerticalBoxSlot* VSlot = VerticalBox_MarketItem->AddChildToVerticalBox(RowBox);
			if (VSlot)
			{
				VSlot->SetHorizontalAlignment(HAlign_Fill);
			}
		}
	}
}

bool ULobbyWidget_Market::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
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
