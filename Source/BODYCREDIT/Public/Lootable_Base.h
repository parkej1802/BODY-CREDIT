// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lootable_Base.generated.h"

UCLASS()
class BODYCREDIT_API ALootable_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootable_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAC_LootingInventoryComponent* LootInventoryComp;
};
