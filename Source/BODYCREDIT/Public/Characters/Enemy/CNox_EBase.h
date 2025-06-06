// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CNox.h"
#include "Components/Enemy/CEnemyState.h"
#include "CNox_EBase.generated.h"

/**
 * Enemy Base
 */
UCLASS()
class BODYCREDIT_API ACNox_EBase : public ACNox
{
	GENERATED_BODY()

public:
	ACNox_EBase();

protected: // Sensing Property
	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float SightRadius = 300.f;
	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float LoseSightRadius = 400.f;
	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float PeripheralVisionAngleDegrees = 45.f;
	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float RetentionTime = 2.f;

	UPROPERTY(EditDefaultsOnly, Category="Sensing")
	float HearingRange = 800.f;

public: // Get Sensing Function
	float GetSightRadius() const { return SightRadius; }
	float GetLoseSightRadius() const { return LoseSightRadius; }
	float GetPeripheralVisionAngleDegrees() const { return PeripheralVisionAngleDegrees; }
	float GetRetentionTime() const { return RetentionTime; }
	float GetHearingRange() const { return HearingRange; }

protected: // Set Sensing Function
	virtual void SetPerceptionInfo()
	{
	}

protected: // Status
	UPROPERTY(EditDefaultsOnly)
	EEnemyType EnemyType;
	UPROPERTY(EditDefaultsOnly)
	float AccelValue = 150.f;

public:
	EEnemyType GetEnemyType() const { return EnemyType; }

protected: // Virtual Function
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
							 class AController* EventInstigator, AActor* DamageCauser) override;

protected: // Component
	UPROPERTY(VisibleDefaultsOnly)
	class UCNox_BehaviorComponent* BehaviorComp;
	UPROPERTY(VisibleDefaultsOnly)
	class UCNoxEnemy_Animinstance* EnemyAnim;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCNoxEnemyHPComponent* HPComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCFSMComponent* FSMComp;

protected:
	UPROPERTY()
	class ACEnemyController* EnemyController;
	void SetApplyDamage(AActor* DamagedPlayer, const float DamageAmout);

public:
	// CCTV에서는 Blackboard에 세팅은 안하고 주변 Enemy에게만 전달한다.
	virtual void SetTarget(ACNox* InTarget);
	ACNox* GetTarget() const { return Target; }
	void SetEnemyState(EEnemyState NewState);
	void SetCombatState(ECombatState NewCombatState);
	void SetTargetCallByDelegate(ACNox* InTarget);
	void HandleAttack(float InAttackDistance); // BT 제거할 때 같이 제거
	void HandleAttack();
	bool IsAttacking();
	bool IsPlayerInDistance();

	virtual void AttackCollision(bool bOn, bool IsRightHand = true)
	{
	}

	void HandleHit(const int32 sectionIdx = 1);
	bool IsHitting();

	void HandleDie(const int32 sectionIdx = 1);
	
	void ResetVal();

protected:
	UPROPERTY(EditDefaultsOnly, Category=Health)
	float HealAmount = 10.f;

public:
	void HealHP();

public: // Medic Android
	void SetGrenadeEnded(bool InbEndedAnim);

private:
	bool bAutoMove = false;
	UPROPERTY(EditDefaultsOnly, Category=AutoMove)
	ACNox* Target = nullptr;
	UPROPERTY(EditDefaultsOnly, Category=AutoMove)
	float MoveDistance = 100.f;
	UPROPERTY(EditDefaultsOnly, Category=AutoMove)
	float AttackDistance = 100.f;

public:
	FORCEINLINE void SetAutoMove(bool InbAutoMove, ACNox* InTarget, float InMoveDistance)
	{
		bAutoMove = InbAutoMove;
		Target = InTarget;
		MoveDistance = InMoveDistance;
	}

	virtual void GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed,
	                                 float& OutNewAccelSpeed)
	{
	};
	void SetMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed);

public:
	bool IsPlayerInForwardRange(ACNox* InTarget, float InForwardRange); // 작업 후 제거 예정
	bool IsPlayerInForwardRange(float InForwardRange);
	bool IsPlayerInForwardDegree(const float InForwardRange, const float InDegree = 10.f);

public:
	void UpdateSkillCoolDowns(ESkillCoolDown Skill, float DeltaTime);
	bool IsSkillReady(ESkillCoolDown Skill) const;
	void UsingSkill(ESkillCoolDown Skill);
};
