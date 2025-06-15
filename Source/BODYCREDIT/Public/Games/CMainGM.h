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
	
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
	UPROPERTY()
	class ACNox_MemoryCollectorAI* MemoryCollectorAI;
	UPROPERTY(EditAnywhere)
	float RegisterPercent = 0.4f;

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
	TArray<ACSpawnBoundaryBox*> ZeroSpawnBoundaryArray;
	UPROPERTY()
	TArray<ACSpawnBoundaryBox*> MedicSpawnBoundaryArray;
	UPROPERTY()
	TArray<ACSpawnBoundaryBox*> MemorySpawnBoundaryArray;
	int32 SpawnBoundaryCount = 0;

	int32 SpawnAbleZeroCount = 0;
	int32 SpawnAbleMedicCount = 0;
	int32 SpawnAbleMemoryCount = 0;

	ACSpawnBoundaryBox* GetSpawnBoundaryBox(const TArray<ACSpawnBoundaryBox*>& SpawnBoundaryBox, int32 SpawnMinFloor);	
	FVector GetSpawnRandomLoc(const ACSpawnBoundaryBox* SpawnBoundaryBox);
	void SpawnEnemy(const TSubclassOf<ACNox_EBase>& SpawnCls, const FVector& SpawnLoc) const;
	bool SpawnBoundaryBoxCheck(const TArray<ACSpawnBoundaryBox*>& SpawnBoundaryBox, const int32 Floor);
public:
	UPROPERTY(BlueprintReadWrite)
	bool ExtractTimerTriggerStart = false;
	bool bSpawnEnemy = false;
	
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();
	UFUNCTION(BlueprintCallable)
	void DestroyEnemy();

public:
	void ChangePlayerStartLocation();
	
}; 
