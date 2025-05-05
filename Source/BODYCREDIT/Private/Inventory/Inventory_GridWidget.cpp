// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Inventory_GridWidget.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"


void UInventory_GridWidget::InitInventory(class UAC_InventoryComponent* InventoryComponent, float Inventoy_TileSize)
{
	InventoryComp = InventoryComponent;
	TileSize = Inventoy_TileSize;
	InventoryRows = InventoryComp->Rows;
	InventoryColumns = InventoryComp->Columns;

	UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Border_Grid->Slot);
	CanvasSlot->SetSize(FVector2D(InventoryColumns * TileSize, InventoryRows * TileSize));

	CreateLineSegment();
}	

void UInventory_GridWidget::CreateLineSegment()
{
	int32 LocalX = 0;
	int32 LocalY = 0;
	FInventoryLine TempLine;

	for (int32 i = 0; i <= InventoryColumns; i++) {
		LocalX = TileSize * i;
		
		TempLine.Start.X = LocalX;
		TempLine.Start.Y = 0;
		TempLine.End.X = LocalX;
		TempLine.End.Y = InventoryRows * TileSize;

		Lines.Add(TempLine);
	}

	for (int32 i = 0; i <= InventoryRows; i++) {
		LocalY = TileSize * i;

		TempLine.Start.X = 0;
		TempLine.Start.Y = LocalY;
		TempLine.End.X = InventoryColumns * TileSize;
		TempLine.End.Y = LocalY;

		Lines.Add(TempLine);
	}
}

int32 UInventory_GridWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	for (const FInventoryLine& Line : Lines)
	{
		TArray<FVector2D> Points;
		Points.Add(Line.Start);
		Points.Add(Line.End);

		FSlateDrawElement::MakeLines(
			OutDrawElements,
			LayerId,
			AllottedGeometry.ToPaintGeometry(),
			Points,
			ESlateDrawEffect::None,
			FLinearColor::White,
			true,
			1.0f
		);
	}

	return LayerId + 1;

}
