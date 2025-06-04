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

private:
	ACEnemyController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;

	void InitPerception();

private:
	UPROPERTY()
	class ACNox_EBase* EnemyBase;
	UPROPERTY()
	class ACNox* TargetPlayer;

public:
	void SetTargetPlayer(ACNox* InTargetPlayer);

private: // Sensing
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* Perception;
	UPROPERTY()
	class UAISenseConfig_Hearing* Hearing;
	UPROPERTY()
	class UAISenseConfig_Sight* Sight;

	// Sensing Delegate Function
	UFUNCTION()
	void OnAITargetPerceptionInfoUpdate(const FActorPerceptionUpdateInfo& UpdateInfo);

private:
	ACNox* GetNearTargetPlayer();

private:
	float CurExpiredTime = 0.f;
	void UpdateExpiredStimuli(float DeltaTime); // Forget Target Player

public: // CCTV BroadCasting
	FDetectPlayer OnDetectPlayer;
};
