// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory_Widget.h"
#include "Inventory/Inventory_GridWidget.h"
#include "Characters/CNox_Runner.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Item/ItemObject.h"
#include "GameState_BodyCredit.h"
#include "Inventory/AC_InventoryComponent.h"

void UInventory_Widget::NativeConstruct()
{
    Super::NativeConstruct();

    APlayerController* PC = GetOwningPlayer();

    APawn* Pawn = PC->GetPawn();

    PlayerCharacter = Cast<ACNox_Runner>(Pawn);

    InventoryComp = PlayerCharacter->InventoryComp;

    InventoryGridWidget->InitInventory(InventoryComp, TileSize);
    InventoryGridWidget->GridID = 0;
    InventoryGridWidget->PlayerController = PC;
    InventoryGridWidget->OwningInventoryWidget = this;

	//Inventory_GridBody->InitEquipment(InventoryComp, 150);
	//Inventory_GridBody->GridID = 2;
	//Inventory_GridBody->PlayerController = PC;
	//Inventory_GridBody->OwningInventoryWidget = this;

    if (bIsLootable) {
        LootingInventoryComp = PlayerCharacter->LootableInventoryComp;
        InventoryItemGridWidget->SetVisibility(ESlateVisibility::Visible);
        InventoryItemGridWidget->InitInventory(LootingInventoryComp, TileSize);
        InventoryItemGridWidget->GridID = 1;
        InventoryItemGridWidget->OwningInventoryWidget = this;
        InventoryItemGridWidget->PlayerController = PC;
    }
    else {

        InventoryItemGridWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UInventory_Widget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    IsMouseOnGrid();
    
}


bool UInventory_Widget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);

    // IsMouseOnGrid();

    return true;
}

void UInventory_Widget::IsMouseOnGrid()
{
    FVector2D MouseScreen;
	/*auto pc = GetWorld()->GetFirstPlayerController();
	if(!pc)
		GEngine->AddOnScreenDebugMessage(
			-1, 1.f, FColor::Green,
			TEXT("!pc"));

	auto mp = pc->GetMousePosition(MouseScreen.X, MouseScreen.Y);
	if (!mp)
		GEngine->AddOnScreenDebugMessage(
			-1, 1.f, FColor::Green,
			TEXT("!mp"));


	if (!GetWorld() ||
		!pc ||
		!mp)
	{
		GEngine->AddOnScreenDebugMessage(
			-1, 1.f, FColor::Green,
			TEXT("IsMouseOnGrid"));
		return;
	}*/

    MouseScreen = FSlateApplication::Get().GetCursorPos() -
        FVector2D(GEngine->GameViewport->GetWindow()->GetPositionInScreen());


    UInventory_GridWidget* NewHoveredGrid = nullptr;

    if (InventoryGridWidget)
    {
        FGeometry GridGeo = InventoryGridWidget->GetGridContentGeometry();
        FVector2D Local = GridGeo.AbsoluteToLocal(MouseScreen);

        if (Local.X >= 0 && Local.Y >= 0&&
            Local.X < GridGeo.GetLocalSize().X &&
            Local.Y < GridGeo.GetLocalSize().Y)
        {
            NewHoveredGrid = InventoryGridWidget;
        }
    }

    if (InventoryItemGridWidget)
    {
        FGeometry GridGeo = InventoryItemGridWidget->GetGridContentGeometry();
        FVector2D Local = GridGeo.AbsoluteToLocal(MouseScreen);

        if (Local.X > 0 && Local.Y >= 0 &&
            Local.X < GridGeo.GetLocalSize().X &&
            Local.Y < GridGeo.GetLocalSize().Y)
        {
            NewHoveredGrid = InventoryItemGridWidget;
        }
    }

    if (CurrentHoveredGrid != NewHoveredGrid)
    {
        CurrentHoveredGrid = NewHoveredGrid;
        if (CurrentHoveredGrid)
        {
            UDragDropOperation* CurrentOp = UWidgetBlueprintLibrary::GetDragDroppingContent();
            if (!CurrentOp) return;

            UItemObject* ItemObject = Cast<UItemObject>(CurrentOp->Payload);
            if (IsValid(ItemObject))
            {
                ItemObject->StartPosition.X = 0;
                ItemObject->StartPosition.Y = 0;
            }

            /*GEngine->AddOnScreenDebugMessage(
                -1, 1.f, FColor::Green,
                FString::Printf(TEXT("Hovered Grid ID: %d"), CurrentHoveredGrid->GridID));*/
        }
        else
        {
			/* GEngine->AddOnScreenDebugMessage(
				 -1, 1.f, FColor::Red, TEXT("No Grid Hovered"));*/
        }
   }
}

FReply UInventory_Widget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    /*FEventReply reply;

    reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

    if (InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton")))
    {
        reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
    }

    return reply.NativeReply;*/
    IsMouseOnGrid();
    return FReply::Handled();
}

bool UInventory_Widget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

    UItemObject* ItemObject = Cast<UItemObject>(InOperation->Payload);

    AGameState_BodyCredit* MyGameState = GetWorld()->GetGameState<AGameState_BodyCredit>();

    MyGameState->SpawnItemFromActor(ItemObject, InventoryComp->GetOwner(), true);

    return true;
}
