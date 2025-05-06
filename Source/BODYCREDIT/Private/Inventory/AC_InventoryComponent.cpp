// Fill out your copyright notice in the Description page of Project Settings.
#include "Inventory/AC_InventoryComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Inventory/Inventory_Widget.h"
#include "Characters/CNox_Runner.h"
#include "Item/ItemObject.h"

// Sets default values for this component's properties
UAC_InventoryComponent::UAC_InventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputAction>TempIA_InventoryMode(TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Inventory.IA_Inventory'"));
	if (TempIA_InventoryMode.Succeeded())
	{
		IA_InventoryMode = TempIA_InventoryMode.Object;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> TempWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Inventory/WBP_MainInventory.WBP_MainInventory'"));
	if (TempWidget.Succeeded())
	{
		InventoryWidget = TempWidget.Class;
	}
}

// Called when the game starts
void UAC_InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* OwnerActor = GetOwner();

	if ((pc = OwnerActor->GetInstigatorController<APlayerController>()) != nullptr)
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(pc->InputComponent);
		if (EnhancedInputComponent)
		{
			SetupInputBinding(EnhancedInputComponent);
		}
	}

	if (OwnerActor)
	{
		PlayerCharacter = Cast<ACNox_Runner>(OwnerActor);
	}

	Items.SetNum(Columns * Rows);
}

// Called every frame
void UAC_InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsDirty)
	{
		IsDirty = false;

	}
}

void UAC_InventoryComponent::SetupInputBinding(class UEnhancedInputComponent* Input)
{
	Input->BindAction(IA_InventoryMode, ETriggerEvent::Started, this, &UAC_InventoryComponent::ShowInventory);
}

void UAC_InventoryComponent::ShowInventory()
{
	if (!bIsInventoryMode) {
		bIsInventoryMode = true;
		if (InventoryWidget)
		{
			InventoryMainUI = CreateWidget<UInventory_Widget>(GetWorld(), InventoryWidget);
		}
		if (InventoryMainUI)
		{
			InventoryMainUI->AddToViewport();
		}
		FInputModeGameAndUI UIInputMode;
		pc->SetInputMode(UIInputMode);
		pc->bShowMouseCursor = true;
	}
	else {
		bIsInventoryMode = false;
		if (InventoryMainUI)
		{
			InventoryMainUI->RemoveFromParent();
		}
		FInputModeGameOnly GameInputMode;
		pc->SetInputMode(GameInputMode);
		pc->bShowMouseCursor = false;
	}
}

bool UAC_InventoryComponent::TryAddItem(UItemObject* ItemObject)
{
	if (IsValid(ItemObject)) 
	{
		for (int32 i = 0; i < Items.Num(); ++i)
		{
			if (IsRoomAvailable(ItemObject, i))
			{
				AddItemAt(ItemObject, i);
				return true;
			}
		}

	}
	return false;
}

FInventoryTile UAC_InventoryComponent::IndexToTile(int32 Index)
{
	FInventoryTile TempTile;

	TempTile.X = Index % Columns;
	TempTile.Y = Index / Columns;

	return TempTile;
}

int32 UAC_InventoryComponent::TileToIndex(FInventoryTile& Tile)
{
	int32 Result;
	return Result = Tile.X + (Tile.Y * Columns);
}

void UAC_InventoryComponent::AddItemAt(class UItemObject* ItemObject, int32 TopLeftIndex)
{
	FInventoryTile ResultTile;

	FInventoryTile TempTile = IndexToTile(TopLeftIndex);
	FIntPoint TempDimension = ItemObject->GetDimension();

	for (int32 i = TempTile.X; i < (TempDimension.X + TempTile.X); ++i)
	{
		for (int32 j = TempTile.Y; j < (TempDimension.Y + TempTile.Y); ++j)
		{
			ResultTile.X = i;
			ResultTile.Y = j;
			if (IsTileValid(ResultTile))
			{
				Items[TileToIndex(ResultTile)] = ItemObject;
				GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Green, FString::Printf(TEXT("Item Index %d"), TileToIndex(ResultTile)));
			}
		}
	}
	IsDirty = true;
}

bool UAC_InventoryComponent::IsRoomAvailable(class UItemObject* ItemObject, int32 TopLeftIndex)
{
	FInventoryTile ResultTile;

	FInventoryTile TempTile = IndexToTile(TopLeftIndex);
	FIntPoint TempDimension = ItemObject->GetDimension();

	for (int32 i = TempTile.X; i < (TempDimension.X + TempTile.X); ++i)
	{
		for (int32 j = TempTile.Y; j < (TempDimension.Y + TempTile.Y); ++j)
		{
			ResultTile.X = i;
			ResultTile.Y = j;

			if (!IsTileValid(ResultTile))
			{
				return false;
			}

			UItemObject* TempItemObject = GetItemAtIndex(TileToIndex(ResultTile));
			if (IsValid(TempItemObject))
			{
				return false;
			}
		}
	}

	return true;
}

void UAC_InventoryComponent::ForEachIndex(class UItemObject* ItemObject, int32 TopLeftIndex)
{
	FInventoryTile ResultTile;

	FInventoryTile TempTile = IndexToTile(TopLeftIndex);
	FIntPoint TempDimension = ItemObject->GetDimension();

	for (int32 i = TempTile.X; i < (TempDimension.X - 1 + TempTile.X); ++i)
	{
		for (int32 j = TempTile.Y; j < (TempDimension.Y - 1 + TempTile.Y); ++j)
		{
			ResultTile.X = i;
			ResultTile.Y = j;
			IsTileValid(ResultTile);
		}
	}
}

bool UAC_InventoryComponent::IsTileValid(FInventoryTile& Tile)
{
	if (Tile.X >= 0 && Tile.Y >= 0 && Tile.X < Columns && Tile.Y < Rows) {
		return true;
	}
	
	return false;
}

UItemObject* UAC_InventoryComponent::GetItemAtIndex(int32 Index)
{
	if (Items.IsValidIndex(Index))
	{
		return Items[Index];
	}

	return nullptr;
}

TMap<UItemObject*, FInventoryTile> UAC_InventoryComponent::GetAllItems()
{
	TMap<UItemObject*, FInventoryTile> AllItem;
	for (int32 i = 0; i < Items.Num(); ++i)
	{
		if (IsValid(Items[i]))
		{
			if (!AllItem.Contains(Items[i]))
			{
				AllItem.Add(Items[i], IndexToTile(i));
			}
		}
	}
	return AllItem;
}

void UAC_InventoryComponent::RemoveItem(UItemObject* ItemObject)
{

}
