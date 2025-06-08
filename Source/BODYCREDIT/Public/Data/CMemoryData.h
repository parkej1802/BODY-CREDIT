// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CMemoryData.generated.h"

UENUM(BlueprintType)
enum class EMemoryTriggerType : uint8
{
	None,
	Stay,
	Combat,
	Looting,
	Max
};

USTRUCT(BlueprintType)
struct FMemoryFragment
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere)
	float TimeStamp = 0.f;

	UPROPERTY(VisibleAnywhere)
	int32 FloorPriority = 0;

	UPROPERTY(VisibleAnywhere)
	int32 CalculatedPriority = 0;

	UPROPERTY(VisibleAnywhere)
	bool bVIPRelated = false;

	UPROPERTY(VisibleAnywhere)
	EMemoryTriggerType TriggerType = EMemoryTriggerType::None;

	UPROPERTY(VisibleAnywhere)
	FName ZoneID = TEXT("");

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class ACNox_Runner> SourcePlayer = nullptr;

	void CalculatePriority(const FVector& AILocation)
	{
		int32 Priority = FloorPriority;

		if (bVIPRelated)
			Priority += 2;

		switch (TriggerType)
		{
		case EMemoryTriggerType::Combat:
			Priority += 1;
			break;
		default:
			break;
		}

		const float Distance = FVector::Dist(Location, AILocation);
		if (Distance > 2000.0f)
			Priority -= 1;

		CalculatedPriority = Priority;
	}

	bool operator<(const FMemoryFragment& Other) const
	{
		return CalculatedPriority > Other.CalculatedPriority; // 높은 값이 우선
	}

	bool operator==(const FMemoryFragment& Other) const
	{
		return TriggerType == Other.TriggerType &&
			   ZoneID == Other.ZoneID &&
			   SourcePlayer == Other.SourcePlayer &&
			   FMath::IsNearlyEqual(TimeStamp, Other.TimeStamp, 0.01f);
	}
};

UENUM(BlueprintType)
enum class EMovementPathType : uint8
{
	None,
	Vent,
	Stairs
};