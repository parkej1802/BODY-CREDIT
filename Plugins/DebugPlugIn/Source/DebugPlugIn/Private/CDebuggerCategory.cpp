#include "CDebuggerCategory.h"

#include "GameFramework/Character.h"
#include "Utilities/CLog.h"
#include "CanvasItem.h"

FCDebuggerCategory::FCDebuggerCategory()
{
	bShowOnlyWithDebugActor = false;
}

FCDebuggerCategory::~FCDebuggerCategory()
{
}

TSharedRef<FGameplayDebuggerCategory> FCDebuggerCategory::MakeInstance()
{
	return MakeShareable(new FCDebuggerCategory());
}

void FCDebuggerCategory::CollectData(APlayerController* OwnerPC, AActor* DebugActor)
{
	FGameplayDebuggerCategory::CollectData(OwnerPC, DebugActor);
	// CLog::Log(TEXT("CollectData"));
	// CLog::Log(OwnerPC);
	// CLog::Log(DebugActor);

	ACharacter* player = OwnerPC->GetPawn<ACharacter>();
	if (!player) return;

	// Player
	{
		PlayerPawnData.bDraw = true;
		PlayerPawnData.Name = player->GetName();
		PlayerPawnData.Location = player->GetActorLocation();
		PlayerPawnData.Forward = player->GetActorForwardVector();
		PlayerPawnData.Speed = player->GetVelocity().Size2D();
	}

	// Forward Actor
	{
		FHitResult hitResult;
		FVector start = PlayerPawnData.Location;
		FVector end = start + PlayerPawnData.Forward * TraceDistance;

		FCollisionQueryParams params;
		params.AddIgnoredActor(player);

		player->GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, params);

		if (hitResult.bBlockingHit)
		{
			ForwardActorData.bDraw = true;
			ForwardActorData.Name = hitResult.GetActor()->GetName();
			ForwardActorData.Location = hitResult.GetActor()->GetActorLocation();
			ForwardActorData.Forward = hitResult.GetActor()->GetActorForwardVector();
		}
		else
		{
			ForwardActorData.bDraw = false;
		}
	}

	// Debug Actor
	if (!!DebugActor)
	{
		DebugActorData.bDraw = true;
		DebugActorData.Name = DebugActor->GetName();
		DebugActorData.Location = DebugActor->GetActorLocation();
		DebugActorData.Forward = DebugActor->GetActorForwardVector();
	}
	else
	{
		DebugActorData.bDraw = false;
	}
}

void FCDebuggerCategory::DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext)
{
	FGameplayDebuggerCategory::DrawData(OwnerPC, CanvasContext);

	FVector start = PlayerPawnData.Location;
	FVector end = start + PlayerPawnData.Forward * TraceDistance;

	DrawDebugLine(OwnerPC->GetWorld(), start, end, FColor::Red);

	// Canvas Settings
	FCanvasTileItem item(FVector2D(10, 10), FVector2D(300, 215),
						FLinearColor(0, 0, 0, .2f));
	item.BlendMode = SE_BLEND_AlphaBlend;
	CanvasContext.DrawItem(item, CanvasContext.CursorX, CanvasContext.CursorY);

	// Canvas Drawing
	CanvasContext.Printf(FColor::Green, L" -- Player Pawn --");
	CanvasContext.Printf(FColor::White, L" -- Name : %s --", *PlayerPawnData.Name);
	CanvasContext.Printf(FColor::White, L" -- Location : %s --", *PlayerPawnData.Location.ToString());
	CanvasContext.Printf(FColor::White, L" -- Forward : %s --", *PlayerPawnData.Forward.ToString());
	CanvasContext.Printf(FColor::White, L" -- Speed : %f --", PlayerPawnData.Speed);
	CanvasContext.Printf(FColor::White, L"");

	if (ForwardActorData.bDraw)
	{
		CanvasContext.Printf(FColor::Green, L" -- Forward Actor --");
		CanvasContext.Printf(FColor::White, L" -- Name : %s --", *ForwardActorData.Name);
		CanvasContext.Printf(FColor::White, L" -- Location : %s --", *ForwardActorData.Location.ToString());
		CanvasContext.Printf(FColor::White, L" -- Forward : %s --", *ForwardActorData.Forward.ToString());
		CanvasContext.Printf(FColor::White, L"");
	}

	if (DebugActorData.bDraw)
	{
		CanvasContext.Printf(FColor::Green, L" -- Select Actor --");
		CanvasContext.Printf(FColor::White, L" -- Name : %s --", *DebugActorData.Name);
		CanvasContext.Printf(FColor::White, L" -- Location : %s --", *DebugActorData.Location.ToString());
		CanvasContext.Printf(FColor::White, L" -- Forward : %s --", *DebugActorData.Forward.ToString());
		CanvasContext.Printf(FColor::White, L"");
	}
}
