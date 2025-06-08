#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CNoxObserverComp.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BODYCREDIT_API UCNoxObserverComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UCNoxObserverComp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	class ACNox_Runner* OwnerCharacter;

private:
	/** 반경 내에서 위치 변화를 무시하는 임계값 (cm) */
	UPROPERTY(EditAnywhere, Category="Stay")
	float PositionTolerance = 30.f;

	/** 머무름을 판정하는 시간 (초) */
	UPROPERTY(EditAnywhere, Category="Stay")
	float RequiredStayTime = 5.f;

	FVector LastRecordedLocation;
	float AccumTime = 0.f;

private:
	// Attacking CoolDown
	UPROPERTY(EditAnywhere, Category="Attack")
	float AttackCooldownSeconds = 10.f;
	bool bIsAttackingCooldown = true;
	FTimerHandle AttackCooldownHandle;
	// Attack Time
	float LastAttackTime = -FLT_MAX;
	void HandleAttackExpired();

	bool CheckAndNotifyIfWithinAttackCooldown() const;

public:
	void RegisterAttack() { LastAttackTime = GetWorld()->GetTimeSeconds(); }

private:
	// Looting CoolDown
	UPROPERTY(EditAnywhere, Category="Looting")
	float LootingCooldownSeconds = 10.f;
	bool bIsLootingCooldown = true;
	FTimerHandle LootingCooldownHandle;
	// Looting Time
	float LastLootingTime = -FLT_MAX;
	void HandleLootingExpired();

	bool CheckAndNotifyIfWithinLootingCooldown() const;

public:
	void RegisterLooting() { LastLootingTime = GetWorld()->GetTimeSeconds(); }
};
