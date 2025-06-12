#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/CMemoryData.h"
#include "CMainGM.generated.h"

class ACNox_EBase;
class ACNox_MedicAndroid;
class ACNox_Zero;
class ACSpawnBoundaryBox;

USTRUCT(BlueprintType)
struct FEnemySpawnData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ZeroSpawnCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MedicSpawnCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MemorySpawnCount = 0;
};

UCLASS()
class BODYCREDIT_API ACMainGM : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACMainGM();

	int32 ItemIndex = 0;

	int32 GetItemIndex();

private:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY()
	class ACNox_MemoryCollectorAI* MemoryCollectorAI;

public:
	void RegisterMemoryFromPlayer(class ACNox_Runner* Player, EMemoryTriggerType Trigger);

private:
	const FString VIPZoneID = "VIP";
	UPROPERTY()
	TArray<class ACAreaTriggerBox*> ZoneVolumes;

	FName GetZoneID(class ACNox_Runner* Player);
	// bool IsInVIPZone(class ACNox_Runner* Player);
	bool IsInVIPZone(const FName& ZoneID);

private:
	float GameStartTime = 0;
public:
	UFUNCTION(BlueprintCallable)
	float GetGamePlayTime(); // 게임 플레이 타임 리턴	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float SetGameTimer = 3.f;

	float GameTimer = 10.f;

	bool bIsFailed = false;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> FailedWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Runner* PlayerCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACNox_Controller* PC;

	bool IsStart = false;

private:
	UPROPERTY(EditDefaultsOnly)
	FEnemySpawnData SpawnData;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACNox_EBase> ZeroFactory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACNox_EBase> MedicFactory;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ACNox_EBase> MemoryFactory;

	UPROPERTY()
	TArray<ACSpawnBoundaryBox*> SpawnBoundaryArray;

	ACSpawnBoundaryBox* GetSpawnBoundaryBox(int32 SpawnMinFloor);	
	FVector GetSpawnRandomLoc(const ACSpawnBoundaryBox* SpawnBoundaryBox);
	void SpawnEnemy(const TSubclassOf<ACNox_EBase>& SpawnCls, const FVector& SpawnLoc) const;
public:
	UPROPERTY(BlueprintReadWrite)
	bool ExtractTimerTriggerStart = false;
	
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
	UFUNCTION(BlueprintCallable)
	void DestroyEnemy();
}; 
