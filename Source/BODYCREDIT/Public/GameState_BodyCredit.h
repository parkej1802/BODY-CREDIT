﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "GameState_BodyCredit.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API AGameState_BodyCredit : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void SpawnItemFromActor(class UItemObject* ItemObject, AActor* Actor, bool GroundClamp);

	void SpawnItemHiddenFromActor(class UItemObject* ItemObject, AActor* Actor, bool GroundClamp);

	void SpawnItemPlayerInventory(class UItemObject* ItemObject, AActor* Actor, bool GroundClamp);

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerController* PC;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;

	UPROPERTY(EditAnywhere)
	class UAC_InventoryComponent* PlayerInventory;

	void RestoreItemAndSubItems(UItemObject* ItemObject, FVector SpawnLocation, AActor* OwnerActor);
};
