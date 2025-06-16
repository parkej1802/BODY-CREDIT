// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "../../../../Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineSessionInterface.h"
#include "Item/ItemObject.h"
#include "Item/ItemDT.h"
#include "AC_LootingInventoryComponent.h"
#include "NetGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGoldChanged, int32, NewGold);

USTRUCT()
struct FItemListByRarity
{
    GENERATED_BODY()

    TArray<FItemData*> Common;
    TArray<FItemData*> Rare;
    TArray<FItemData*> Epic;
    TArray<FItemData*> Legendary;
};

USTRUCT(BlueprintType)
struct FRarityProbability
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Common = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rare = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Epic = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Legendary = 0.05f;
};

UCLASS()
class BODYCREDIT_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* StageDataTable;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundCue* BGM_Cue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundMix* BGM_Mix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundClass* MasterBGMClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundClass* LobbySoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundClass* InGameSoundClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USoundClass* ExtractSoundClass;

	void InitMix();
	void SetLobbySound(float Volume = 1.f, float Pitch = 1.f);
	void SetInGameSound(float Volume = 1.f, float Pitch = 1.f);
	void SetExtractSound(float Volume = 1.f, float Pitch = 1.f);

	UFUNCTION(BlueprintCallable)
	USoundCue* GetBGM();

	UFUNCTION(BlueprintCallable)
	void PlayBGM(const int32 idx);
	UFUNCTION(BlueprintCallable)
	void StopBGM();
	void PauseBGM(bool bIsPause);

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
	int32 PlayerGold = 50000;

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

	UPROPERTY()
	int32 RemainingLife = 4;

	UPROPERTY()
	int32 DayLeft = -1;

	UPROPERTY()
	int32 AmountToPay = 0;

	UPROPERTY()
	bool PayTime = false;

	UPROPERTY()
	bool Failed = false;

	UPROPERTY()
	int32 InitialPlayerGold = 0;

	void RefreshGame();

	UFUNCTION(BlueprintCallable, Category="Day")
	int32 GetDay() const { return Day; }

	virtual void PostInitProperties() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRarityProbability RarityRate;

	EItemRarity GetRandomRarity();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ELootBoxTier, FRarityProbability> LootBoxRarityMap;

	EItemRarity GetRandomRarityByLootTier(ELootBoxTier Tier);

	UPROPERTY()
	FItemListByRarity CachedItemLists;

	void InitItemCache(UDataTable* ItemDT);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxNumKey = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumKeySpawned = 0;
}; 
