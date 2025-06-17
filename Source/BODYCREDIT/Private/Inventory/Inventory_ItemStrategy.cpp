#include "Inventory/Inventory_ItemStrategy.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/DragDropOperation.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Inventory/Inventory_ItemInventoryWidget.h"
#include "Item/ItemObject.h"
#include "Components/CanvasPanel.h"
#include "Session/NetGameInstance.h"

void UInventory_ItemStrategy::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_Exit)
	{
		Button_Exit->OnClicked.AddDynamic(this, &UInventory_ItemStrategy::OnExitClicked);
	}

	if (Button_Drag)
	{
		Button_Drag->OnPressed.AddDynamic(this, &UInventory_ItemStrategy::OnDragPressed);
		Button_Drag->OnReleased.AddDynamic(this, &UInventory_ItemStrategy::OnDragReleased);
	}

	if (UNetGameInstance* GI = Cast<UNetGameInstance>(GetWorld()->GetGameInstance()))
	{
		GI->OnBack.RemoveAll(this);
		GI->OnBack.AddUObject(this, &UInventory_ItemStrategy::RemoveWidget);
	}

}

void UInventory_ItemStrategy::OnExitClicked()
{
	ItemObject->bIsUseFunction = !ItemObject->bIsUseFunction;
	RemoveFromParent();
}

void UInventory_ItemStrategy::OnDragPressed()
{
	FVector2D MouseScreen;
	FVector2D ViewportPos;

	MouseScreen = FSlateApplication::Get().GetCursorPos();
	ViewportPos = GEngine->GameViewport->GetWindow()->GetPositionInScreen();

	FVector2D LocalMousePos = MouseScreen - ViewportPos;

	const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
	FVector2D ScreenPos = LocalMousePos / Scale;

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(CanvasPanel_ItemStrategy->Slot))
	{
		FVector2D WidgetPos = CanvasSlot->GetPosition();
		MouseOffset = ScreenPos - WidgetPos;
		bIsDragging = true;
	}
}

void UInventory_ItemStrategy::OnDragReleased()
{
	bIsDragging = false;
}

void UInventory_ItemStrategy::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bIsDragging && CanvasPanel_ItemStrategy)
	{
		FVector2D MouseScreen;
		FVector2D ViewportPos;

		MouseScreen = FSlateApplication::Get().GetCursorPos();
		ViewportPos = GEngine->GameViewport->GetWindow()->GetPositionInScreen();

		FVector2D LocalMousePos = MouseScreen - ViewportPos;
		const float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		FVector2D NewPos = (LocalMousePos / Scale) - MouseOffset;

		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(CanvasPanel_ItemStrategy->Slot))
		{
			CanvasSlot->SetPosition(NewPos);
		}
	}
}

void UInventory_ItemStrategy::RemoveWidget()
{
	ItemObject->bIsUseFunction = !ItemObject->bIsUseFunction;
	RemoveFromParent();
}

