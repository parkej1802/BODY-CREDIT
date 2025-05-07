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

