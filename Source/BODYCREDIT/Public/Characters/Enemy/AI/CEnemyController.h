// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CEnemyController.generated.h"

/**
 * Enemy AI Controller
 */
UCLASS()
class BODYCREDIT_API ACEnemyController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class ACNox_EBase* EnemyBase;

	UPROPERTY()
	class UCNox_BehaviorComponent* BT_Behavior;

	UPROPERTY(EditDefaultsOnly, Category = "BehaviorTree")
	UBehaviorTree* NoxBehaviorTree;
	
private: // Sensing
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* Perception;

	UPROPERTY()
	class UAISenseConfig_Hearing* Hearing;

	UPROPERTY()
	class UAISenseConfig_Sight* Sight;

private: // Sensing Delegate Function
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);
	
private:
	ACEnemyController();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;

	void InitPerception();
};
