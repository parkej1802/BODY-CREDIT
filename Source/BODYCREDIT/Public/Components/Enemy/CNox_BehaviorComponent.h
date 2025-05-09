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
	const FName TargetPlayer = "Target";
	
	const FName PatrolRoute = "PatrolRoute";
	
	const FName GrenadeKey = "GrenadeAnimEnd";
	const FName ShieldKey = "ShieldAnimEnd";
	const FName HealFlagKey = "bHealFlag";
	const FName EquipShieldKey = "bIsEquipShield";
	
private:
	UPROPERTY()
	class UBlackboardComponent* Blackboard;
	
public:
	FORCEINLINE void SetBlackboard(UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

public:
	void SetEnemyType(EEnemyType InType);
	void SetTarget(class ACNox* InTarget);

public:	// For Zero
	void SetPatrolRoute(class ACPatrolRoute* InPatrolRoute);

public:	// For Medic Android
	void SetGrenadeEnded(bool InbEndedAnim);
	void SetEquipShield(bool bInEquipShield);
	void SetShieldEnded(bool InbEndedAnim);
	void SetHealFlag(bool bInHealFlag);
};
