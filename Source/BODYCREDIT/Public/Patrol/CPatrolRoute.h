// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "CPatrolRoute.generated.h"

UCLASS()
class BODYCREDIT_API ACPatrolRoute : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPatrolRoute();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	class USceneComponent* RootComp;
	UPROPERTY()
	class USplineComponent* SplineComp;

public:
	int32 PatrolRouteIndex{0};
	int32 Direction{1};
	void IncrementPatrolRoute();
	FVector GetNextPointAsPosition();
	int32 GetNumberOfSplinePoints() const { return SplineComp->GetNumberOfSplinePoints(); }
	float GetDistanceAlongSplineAtSplinePoint(const int32 idx) const { return SplineComp->GetDistanceAlongSplineAtSplinePoint(idx); }
	FVector GetLocationAtDistanceAlongSpline(const float InDistance, const ESplineCoordinateSpace::Type& CoordinateSpace) const;
};
