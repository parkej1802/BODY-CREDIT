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
#include "Session/NetGameInstance.h"
#include "Components/TextBlock.h"
#include "Lobby/LobbyWidget_SellItem.h"
#include "Lobby/LobbyWidget_BuyItem.h"
#include "GameState_BodyCredit.h"
#include "Lobby/LobbyWidget_NoSpace.h"

void ULobbyWidget_Market::NativeConstruct()
{
    Super::NativeConstruct();

    if (Button_Back)
    {
        Button_Back->OnClicked.AddDynamic(this, &ThisClass::OnBackClicked);
		Button_Back->OnHovered.AddDynamic(this, &ThisClass::OnBackHovered);
		Button_Back->OnUnhovered.AddDynamic(this, &ThisClass::OnBackUnhovered);
    }

    if (Button_SelectWeapon)
    {
        Button_SelectWeapon->OnClicked.AddDynamic(this, &ThisClass::OnSelectWeaponClicked);
		Button_SelectWeapon->OnHovered.AddDynamic(this, &ThisClass::OnSelectWeaponHovered);
		Button_SelectWeapon->OnUnhovered.AddDynamic(this, &ThisClass::OnSelectWeaponUnhovered);
    }

    if (Button_SelectHead)
    {
        Button_SelectHead->OnClicked.AddDynamic(this, &ThisClass::OnSelectHeadClicked);
		Button_SelectHead->OnHovered.AddDynamic(this, &ThisClass::OnSelectHeadHovered);
		Button_SelectHead->OnUnhovered.AddDynamic(this, &ThisClass::OnSelectHeadUnhovered);
    }

    if (Button_SelectBody)
    {
        Button_SelectBody->OnClicked.AddDynamic(this, &ThisClass::OnSelectBodyClicked);
		Button_SelectBody->OnHovered.AddDynamic(this, &ThisClass::OnSelectBodyHovered);
		Button_SelectBody->OnUnhovered.AddDynamic(this, &ThisClass::OnSelectBodyUnhovered);
    }

    if (Button_SelectArm)
    {
        Button_SelectArm->OnClicked.AddDynamic(this, &ThisClass::OnSelectArmClicked);
		Button_SelectArm->OnHovered.AddDynamic(this, &ThisClass::OnSelectArmHovered);
		Button_SelectArm->OnUnhovered.AddDynamic(this, &ThisClass::OnSelectArmUnhovered);
    }

    if (Button_SelectLeg)
    {
        Button_SelectLeg->OnClicked.AddDynamic(this, &ThisClass::OnSelectLegClicked);
		Button_SelectLeg->OnHovered.AddDynamic(this, &ThisClass::OnSelectLegHovered);
		Button_SelectLeg->OnUnhovered.AddDynamic(this, &ThisClass::OnSelectLegUnhovered);
    }

    if (Button_SelectBackpack)
    {
        Button_SelectBackpack->OnClicked.AddDynamic(this, &ThisClass::OnSelectBackpackClicked);
		Button_SelectBackpack->OnHovered.AddDynamic(this, &ThisClass::OnSelectBackpackHovered);
		Button_SelectBackpack->OnUnhovered.AddDynamic(this, &ThisClass::OnSelectBackpackUnhovered);
    }

	if (Button_SelectChestrig)
	{
		Button_SelectChestrig->OnClicked.AddDynamic(this, &ThisClass::OnSelectChestrigClicked);
		Button_SelectChestrig->OnHovered.AddDynamic(this, &ThisClass::OnSelectChestrigHovered);
		Button_SelectChestrig->OnUnhovered.AddDynamic(this, &ThisClass::OnSelectChestrigUnhovered);
	}

	
	Refresh();
}

void ULobbyWidget_Market::OnBackClicked()
{
    if (LobbySelectionWidgetClass)
    {
        LobbyWidget_Selection = CreateWidget<ULobbyWidget_Selection>(GetWorld(), LobbySelectionWidgetClass);
        if (LobbyWidget_Selection)
        {
            LobbyWidget_Selection->AddToViewport();

            RemoveFromParent();
        }
    }
}

void ULobbyWidget_Market::OnBackHovered()
{
	if (Image_Button_Back_Hovered)
	{
		Image_Button_Back_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyWidget_Market::OnBackUnhovered()
{
	if (Image_Button_Back_Hovered)
	{
		Image_Button_Back_Hovered->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULobbyWidget_Market::OnSelectWeaponClicked()
{
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
	{
		return;
	}

	if (PreviousImage == Image_SelectWeapon_Hovered) return;
	
	TurnOffPreviousImage();
	PreviousImage = Image_SelectWeapon_Hovered;
	TurnOnPreviousImage();
	

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
			ItemWidget->OwningMarket = this;
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

void ULobbyWidget_Market::OnSelectWeaponHovered()
{
	if (PreviousImage == Image_SelectWeapon_Hovered)
	{
		return;
	}

	if (Image_SelectWeapon_Hovered)
	{
		Image_SelectWeapon_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}


void ULobbyWidget_Market::OnSelectWeaponUnhovered()
{
	if (PreviousImage == Image_SelectWeapon_Hovered)
	{
		return;
	}

	if (Image_SelectWeapon_Hovered)
	{
		Image_SelectWeapon_Hovered->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULobbyWidget_Market::OnSelectHeadClicked()
{
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
	{
		return;
	}

	if (PreviousImage == Image_SelectHead_Hovered) return;

	TurnOffPreviousImage();
	PreviousImage = Image_SelectHead_Hovered;
	TurnOnPreviousImage();

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
			ItemWidget->OwningMarket = this;
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

void ULobbyWidget_Market::OnSelectHeadHovered()
{
	if (PreviousImage == Image_SelectHead_Hovered)
	{
		return;
	}
	if (Image_SelectHead_Hovered)
	{
		Image_SelectHead_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyWidget_Market::OnSelectHeadUnhovered()
{
	if (PreviousImage == Image_SelectHead_Hovered)
	{
		return;
	}
	if (Image_SelectHead_Hovered)
	{
		Image_SelectHead_Hovered->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULobbyWidget_Market::OnSelectBodyClicked()
{
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
	{
		return;
	}

	if (PreviousImage == Image_SelectBody_Hovered) return;

	TurnOffPreviousImage();
	PreviousImage = Image_SelectBody_Hovered;
	TurnOnPreviousImage();

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
			ItemWidget->OwningMarket = this;
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

void ULobbyWidget_Market::OnSelectBodyHovered()
{
	if (PreviousImage == Image_SelectBody_Hovered)
	{
		return;
	}
	if (Image_SelectBody_Hovered)
	{
		Image_SelectBody_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyWidget_Market::OnSelectBodyUnhovered()
{
	if (PreviousImage == Image_SelectBody_Hovered)
	{
		return;
	}
	if (Image_SelectBody_Hovered)
	{
		Image_SelectBody_Hovered->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULobbyWidget_Market::OnSelectArmClicked()
{
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
	{
		return;
	}

	if (PreviousImage == Image_SelectArm_Hovered) return;

	TurnOffPreviousImage();
	PreviousImage = Image_SelectArm_Hovered;
	TurnOnPreviousImage();

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
			ItemWidget->OwningMarket = this;
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

void ULobbyWidget_Market::OnSelectArmHovered()
{
	if (PreviousImage == Image_SelectArm_Hovered)
	{
		return;
	}
	if (Image_SelectArm_Hovered)
	{
		Image_SelectArm_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyWidget_Market::OnSelectArmUnhovered()
{
	if (PreviousImage == Image_SelectArm_Hovered)
	{
		return;
	}
	if (Image_SelectArm_Hovered)
	{
		Image_SelectArm_Hovered->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULobbyWidget_Market::OnSelectLegClicked()
{
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
	{
		return;
	}

	if (PreviousImage == Image_SelectLeg_Hovered) return;

	TurnOffPreviousImage();
	PreviousImage = Image_SelectLeg_Hovered;
	TurnOnPreviousImage();

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
			ItemWidget->OwningMarket = this;
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

void ULobbyWidget_Market::OnSelectLegHovered()
{
	if (PreviousImage == Image_SelectLeg_Hovered)
	{
		return;
	}

	if (Image_SelectLeg_Hovered)
	{
		Image_SelectLeg_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyWidget_Market::OnSelectLegUnhovered()
{
	if (PreviousImage == Image_SelectLeg_Hovered)
	{
		return;
	}

	if (Image_SelectLeg_Hovered)
	{
		Image_SelectLeg_Hovered->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULobbyWidget_Market::OnSelectBackpackClicked()
{
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
	{
		return;
	}


	if (PreviousImage == Image_SelectBackpack_Hovered) return;

	TurnOffPreviousImage();
	PreviousImage = Image_SelectBackpack_Hovered;
	TurnOnPreviousImage();

	VerticalBox_MarketItem->ClearChildren();

	TArray<FItemData> Weapons = MarketComp->GetMarketItems(EPlayerPart::Backpack);
	/*TArray<FItemData> ChestRigs = MarketComp->GetMarketItems(EPlayerPart::ChestRigs);

	Weapons.Append(ChestRigs);*/

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
			ItemWidget->OwningMarket = this;
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

void ULobbyWidget_Market::OnSelectBackpackHovered()
{
	if (PreviousImage == Image_SelectBackpack_Hovered)
	{
		return;
	}

	if (Image_SelectBackpack_Hovered)
	{
		Image_SelectBackpack_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyWidget_Market::OnSelectBackpackUnhovered()
{
	if (PreviousImage == Image_SelectBackpack_Hovered)
	{
		return;
	}

	if (Image_SelectBackpack_Hovered)
	{
		Image_SelectBackpack_Hovered->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULobbyWidget_Market::OnSelectChestrigClicked()
{
	if (!MarketComp || !MarketItemWidget || !VerticalBox_MarketItem)
	{
		return;
	}

	if (PreviousImage == Image_SelectChestrig_Hovered) return;

	TurnOffPreviousImage();
	PreviousImage = Image_SelectChestrig_Hovered;
	TurnOnPreviousImage();

	VerticalBox_MarketItem->ClearChildren();

	TArray<FItemData> Weapons = MarketComp->GetMarketItems(EPlayerPart::ChestRigs);

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
			ItemWidget->OwningMarket = this;
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

void ULobbyWidget_Market::OnSelectChestrigHovered()
{
	if (PreviousImage == Image_SelectChestrig_Hovered)
	{
		return;
	}

	if (Image_SelectChestrig_Hovered)
	{
		Image_SelectChestrig_Hovered->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyWidget_Market::OnSelectChestrigUnhovered()
{
	if (PreviousImage == Image_SelectChestrig_Hovered)
	{
		return;
	}

	if (Image_SelectChestrig_Hovered)
	{
		Image_SelectChestrig_Hovered->SetVisibility(ESlateVisibility::Hidden);
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

	ShowSellUI(ItemObject);

	return true;
}

void ULobbyWidget_Market::UpdatePlayerGoldText(int32 NewGold)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("UpdatePlayerGoldText"));
	if (Text_PlayerGold)
	{
		Text_PlayerGold->SetText(FText::AsNumber(NewGold));
	}
}

void ULobbyWidget_Market::ShowSellUI(UItemObject* ItemObject)
{
	if (SellItemUI)
	{
		SellItemUI = nullptr;
	}

	if (SellItemWidget)
	{
		SellItemUI = CreateWidget<ULobbyWidget_SellItem>(this, SellItemWidget);

	}
	if (SellItemUI)
	{
		SellItemUI->MarketUI = this;
		SellItemUI->SetItemToSell(ItemObject);
		SellItemUI->AddToViewport();
		SellItemUI->OnConfirmSell.Clear();
		SellItemUI->OnCancelSell.Clear();
		SellItemUI->OnConfirmSell.AddDynamic(this, &ULobbyWidget_Market::HandleSellConfirm);
		SellItemUI->OnCancelSell.AddDynamic(this, &ULobbyWidget_Market::HandleSellCancel);

	}
}

void ULobbyWidget_Market::HandleSellConfirm(UItemObject* ItemObject)
{
	if (!ItemObject) return;

	int32 SellPrice = ItemObject->GetSellPrice();
	int32 NewGold = GI->PlayerGold + SellPrice;
	GI->SetPlayerGold(NewGold);

	ItemObject->OwnerInventoryComp->RemoveItem(ItemObject);
}

void ULobbyWidget_Market::HandleSellCancel(class UItemObject* ItemObject)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("CancelSell"));
	if (ItemObject)
	{
		ItemObject->OwnerInventoryComp->TryAddItem(ItemObject);
	}
}

void ULobbyWidget_Market::TurnOffPreviousImage()
{
	if (PreviousImage)
	{
		PreviousImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ULobbyWidget_Market::TurnOnPreviousImage()
{
	if (PreviousImage)
	{
		PreviousImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULobbyWidget_Market::ShowBuyUI(UItemObject* ItemObject)
{
	if (BuyItemUI)
	{
		BuyItemUI = nullptr;
	}

	if (BuyItemWidget)
	{
		BuyItemUI = CreateWidget<ULobbyWidget_BuyItem>(this, BuyItemWidget);

	}
	if (BuyItemUI)
	{
		BuyItemUI->MarketUI = this;
		BuyItemUI->SetItemToBuy(ItemObject);
		BuyItemUI->AddToViewport();
		BuyItemUI->OnConfirmBuy.Clear();
		BuyItemUI->OnCancelBuy.Clear();
		BuyItemUI->OnConfirmBuy.AddDynamic(this, &ULobbyWidget_Market::HandleBuyConfirm);
		BuyItemUI->OnCancelBuy.AddDynamic(this, &ULobbyWidget_Market::HandleBuyCancel);

	}
}

void ULobbyWidget_Market::HandleBuyConfirm(class UItemObject* ItemObject)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("ConfirmBuy"));

	if (GameState)
	{
		if (ItemObject->ItemData.ItemType != EPlayerPart::ChestRigs && ItemObject->ItemData.ItemType != EPlayerPart::Backpack
			&& ItemObject->ItemData.ItemType != EPlayerPart::Basic)
		{
			ItemObject->ItemData.Rarity = GI->GetRandomRarity();
			ItemObject->SetIconImages();
			ItemObject->SetItemStat();
		}

		if (GameState->SpawnItemPlayerInventory(ItemObject, GetOwningPlayerPawn(), false))
		{
			int32 NewGold = GI->PlayerGold - ItemObject->ItemData.Price;
			GI->SetPlayerGold(NewGold);

			UpdatePlayerGoldText(NewGold);
		}
		else
		{
			ShowNoSpaceUI();
			ItemObject = nullptr;
		}
	}
}

void ULobbyWidget_Market::HandleBuyCancel(class UItemObject* ItemObject)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("CancelBuy"));
	ItemObject = nullptr;
}

void ULobbyWidget_Market::ShowNoSpaceUI()
{
	if (NoSpaceWidget)
	{
		NoSpaceUI = CreateWidget<ULobbyWidget_NoSpace>(this, NoSpaceWidget);

	}
	if (NoSpaceUI)
	{
		NoSpaceUI->AddToViewport();
	}
}

void ULobbyWidget_Market::Refresh()
{
	PC = Cast<ACNox_Controller>(GetOwningPlayer());
	FInputModeGameAndUI InputMode;
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;

	APawn* Pawn = PC->GetPawn();

	PlayerCharacter = Cast<ACNox_Runner>(Pawn);

	InventoryComp = PlayerCharacter->InventoryComp;

	InventoryGridWidget->InitInventory(InventoryComp, InventoryComp->InventoryTileSize);
	InventoryGridWidget->GridID = 0;
	InventoryGridWidget->PlayerController = PC;

	MarketComp = PlayerCharacter->MarketComp;

	OnSelectWeaponClicked();
	PreviousImage = Image_SelectWeapon_Hovered;
	PreviousImage->SetVisibility(ESlateVisibility::Visible);

	GameState = GetWorld()->GetGameState<AGameState_BodyCredit>();

	GI = Cast<UNetGameInstance>(GetGameInstance());
	if (GI) {
		GI->MarketUI = this;
		GI->OnGoldChanged.RemoveDynamic(this, &ULobbyWidget_Market::UpdatePlayerGoldText);
		GI->OnGoldChanged.AddDynamic(this, &ULobbyWidget_Market::UpdatePlayerGoldText);
		GI->SetPlayerGold(GI->PlayerGold);
		// UpdatePlayerGoldText(GI->PlayerGold);
	}
}
