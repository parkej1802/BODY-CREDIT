// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CStair.generated.h"

UCLASS()
class BODYCREDIT_API ACStair : public AActor
{
	GENERATED_BODY()
	
public:	
	ACStair();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* PointA;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* PointB;

public:
	// 현재 위치 기준으로 가까운 쪽을 Entry로, 나머지를 Exit로 판단
	void GetClosestEntryAndExit(const FVector& FromLocation, FVector& OutEntry, FVector& OutExit) const;
};
