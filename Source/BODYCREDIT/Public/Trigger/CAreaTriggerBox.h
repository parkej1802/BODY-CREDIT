// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CAreaTriggerBox.generated.h"

UCLASS()
class BODYCREDIT_API ACAreaTriggerBox : public AActor
{
	GENERATED_BODY()

public:
	ACAreaTriggerBox();
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComp;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category="ZoneID")
	FName ZoneID = TEXT("Normal");
};
