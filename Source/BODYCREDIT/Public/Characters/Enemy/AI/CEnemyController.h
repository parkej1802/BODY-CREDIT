#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CEnemyController.generated.h"

struct FActorPerceptionUpdateInfo;
DECLARE_DELEGATE_OneParam(FDetectPlayer, class ACNox*);

/**
 * Enemy AI Controller
 */
UCLASS()
class BODYCREDIT_API ACEnemyController : public AAIController
{
	GENERATED_BODY()

#pragma region Common

private:
	UPROPERTY()
	class ACNox_EBase* EnemyBase;
	UPROPERTY()
	class ACNox* TargetPlayer;
#pragma endregion

#pragma region Init

private:
	ACEnemyController();
	virtual void OnPossess(APawn* InPawn) override;
	void InitPerception();
#pragma endregion

#pragma region Sensing

private:
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* Perception;
	UPROPERTY()
	class UAISenseConfig_Hearing* Hearing;
	UPROPERTY()
	class UAISenseConfig_Sight* Sight;

	// Sensing Delegate Function
	UFUNCTION()
	void OnAITargetPerceptionInfoUpdate(const FActorPerceptionUpdateInfo& UpdateInfo);
#pragma endregion

#pragma region Get Near Player

private:
	ACNox* GetNearTargetPlayer();
#pragma endregion

#pragma region Tick

private:
	virtual void Tick(float DeltaSeconds) override;
#pragma endregion

#pragma region Set Target

public:
	void SetTargetPlayer(ACNox* InTargetPlayer);
#pragma endregion

#pragma region Target loss

private:
	float CurExpiredTime = 0.f;
	void UpdateExpiredStimuli(float DeltaTime);
#pragma endregion

#pragma region CCTV BroadCasting

public:
	FDetectPlayer OnDetectPlayer;
#pragma endregion

#pragma region Stop Perception (Using Die)

public:
	void PerceptionDeactive();
#pragma endregion
};
