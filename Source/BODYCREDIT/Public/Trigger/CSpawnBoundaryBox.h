#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSpawnBoundaryBox.generated.h"

USTRUCT(BlueprintType)
struct FEnemySpawnAbleCount
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ZeroSpawnCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MedicSpawnCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MemorySpawnCount = 0;

	int32 CurZeroSpawnCount = 0;
	int32 CurMedicSpawnCount = 0;
	int32 CurMemorySpawnCount = 0;
};

UCLASS()
class BODYCREDIT_API ACSpawnBoundaryBox : public AActor
{
	GENERATED_BODY()

public:
	ACSpawnBoundaryBox();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere)
	int32 floor = 1;

public:
	int32 GetFloor() const { return floor; }
	FVector GetLoc() const;
	FVector GetScale() const;

public:
	void AppendZeroCount();
	void AppendMedicCount();
	void AppendMemoryCount();
	void ResetCount();

	bool CanSpawnZero();
	bool CanSpawnMedic();
	bool CanSpawnMemory();

	int32 GetSpawnAbleZeroCount() const { return SpawnCount.ZeroSpawnCount; }
	int32 GetSpawnAbleMedicCount() const { return SpawnCount.MedicSpawnCount; }
	int32 GetSpawnAbleMemoryCount() const { return SpawnCount.MemorySpawnCount; }

private:
	UPROPERTY(EditAnywhere)
	FEnemySpawnAbleCount SpawnCount;
};
