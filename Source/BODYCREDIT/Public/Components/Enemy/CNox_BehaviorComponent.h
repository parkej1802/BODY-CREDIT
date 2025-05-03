// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Components/ActorComponent.h"
#include "CNox_BehaviorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYCREDIT_API UCNox_BehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCNox_BehaviorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	const FName AIStateTypeKey = "AIType";
	const FName PatrolRoute = "PatrolRoute";
	
private:
	UPROPERTY()
	class UBlackboardComponent* Blackboard;
	
public:
	FORCEINLINE void SetBlackboard(UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

public:
	void SetEnemyType(EEnemyType InType);

public:	// For Zero
	void SetPatrolRoute(class ACPatrolRoute* InPatrolRoute);
};
