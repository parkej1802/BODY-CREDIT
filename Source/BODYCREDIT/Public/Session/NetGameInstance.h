// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "Item/ItemObject.h"
#include "NetGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	
	virtual void Init() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;

public:
	IOnlineSessionPtr sessionInterface;

	void CreateMySession(FString roomName, int32 playerCount);

	FString mySessionName = "BodyCredit";

	UFUNCTION()
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	UPROPERTY()
	TMap<EPlayerPart, FItemSaveData> SavedEquippedItems;

	UPROPERTY()	
	TArray<FItemSaveData> SavedInventoryItems;
};
