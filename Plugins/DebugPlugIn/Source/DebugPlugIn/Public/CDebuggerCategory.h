#pragma once

#include "CoreMinimal.h"
#include "GameplayDebuggerCategory.h"

/**
 * 
 */
class DEBUGPLUGIN_API FCDebuggerCategory : public FGameplayDebuggerCategory
{
public:
	FCDebuggerCategory();
	~FCDebuggerCategory();

public:
	static TSharedRef<FGameplayDebuggerCategory> MakeInstance();

public:
	virtual void CollectData(APlayerController* OwnerPC, AActor* DebugActor) override;
	virtual void DrawData(APlayerController* OwnerPC, FGameplayDebuggerCanvasContext& CanvasContext) override;

private:
	struct FCategoryData
	{
		bool bDraw = false;
		FString Name;
		FVector Location;
		FVector Forward;
		float Speed;
	};

private:
	FCategoryData PlayerPawnData;
	FCategoryData ForwardActorData;
	FCategoryData DebugActorData;

private:
	float TraceDistance = 500.f;
};
