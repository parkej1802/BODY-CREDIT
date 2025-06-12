// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "Item/ItemObject.h"
#include "NetGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldChanged, int32, NewGold);

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

	UPROPERTY()
	int32 Debt = 10000000;

	UPROPERTY()
	int32 PlayerGold = 5000;

	UPROPERTY(BlueprintAssignable)
	FOnGoldChanged OnGoldChanged;

	UFUNCTION(BlueprintCallable)
	void SetPlayerGold(int32 NewGold);

	UPROPERTY()	
	class ULobbyWidget_Market* MarketUI;

	int32 BeforePlayerGold = 0;
	int32 AfterPlayerGold = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APlayerController* PC;

	void SetActorInitLocation();

	int32 Day = 1;

	TMap<EPlayerPart, bool> AlivePart;

	EPlayerPart SelectedPart = EPlayerPart::Basic;

	UPROPERTY()
	class UImage* SaveImageHead;

	UPROPERTY()
	class UImage* SaveImageBody;

	UPROPERTY()
	class UImage* SaveImageArm;

	UPROPERTY()
	class UImage* SaveImageLeg;
};
