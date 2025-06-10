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

#pragma region Init
public:
	ACNox_EBase();

protected: // Virtual Function
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;

protected:
	UPROPERTY()
	class ACEnemyController* EnemyController;
	UPROPERTY(EditAnywhere, Category=Debug)
	bool bDebug = true;

	void InitComp();
#pragma endregion

#pragma region Component
protected:
	UPROPERTY(VisibleDefaultsOnly)
	class UCNoxEnemy_Animinstance* EnemyAnim;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCNoxEnemyHPComponent* HPComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCFSMComponent* FSMComp;
#pragma endregion

#pragma region Sensing
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
#pragma endregion

#pragma region EnemyType
protected:
	UPROPERTY(EditDefaultsOnly)
	EEnemyType EnemyType;

public:
	EEnemyType GetEnemyType() const { return EnemyType; }
#pragma endregion

#pragma region Apply Damage
public:
	void SetApplyDamage(AActor* DamagedPlayer, const float DamageAmout);
#pragma endregion

#pragma region Target

protected:
	UPROPERTY(EditDefaultsOnly)
	ACNox* Target = nullptr;

public:
	virtual void SetTarget(ACNox* InTarget);
	ACNox* GetTarget() const { return Target; }
#pragma endregion
	
#pragma region Set Movement Speed

protected:
	virtual void GetNewMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed, float& OutNewSpeed,
									 float& OutNewAccelSpeed)
	{
	};

public:
	void SetMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed);
#pragma endregion
	
#pragma region Attacking

public:
	void HandleAttack();
	bool IsAttacking();

	virtual void AttackCollision(bool bOn, bool IsRightHand = true)
	{
	}
#pragma endregion

#pragma region BroadCast To Near Enemy

public:
	void SetTargetCallByDelegate(ACNox* InTarget);
#pragma endregion

#pragma region Hitting

public:
	void HandleHit(const int32 sectionIdx = 1);
	bool IsHitting();
	void ResetVal() const;
#pragma endregion

#pragma region Die

public:
	void HandleDie(const int32 sectionIdx = 1);
#pragma endregion

#pragma region Heal (Medic)

protected:
	UPROPERTY(EditDefaultsOnly, Category=Health)
	float HealAmount = 10.f;

public:
	void HealHP();
#pragma endregion

#pragma region Check Player In Forward Degree

public:
	bool IsPlayerInForwardDegree(const float InForwardRange, const float InDegree = 10.f);
#pragma endregion

#pragma region FSM Set State

public:
	void SetEnemyState(EEnemyState NewState);
	void SetCombatState(ECombatState NewCombatState);
#pragma endregion

#pragma region FSM Skill Cool Downs

public:
	void UpdateSkillCoolDowns(ESkillCoolDown Skill, float DeltaTime);
	bool IsSkillReady(ESkillCoolDown Skill) const;
	void UsingSkill(ESkillCoolDown Skill);
#pragma endregion

#pragma region Rotate To Targer
public:
	bool RotateToTarget(const float DeltaTime, const FTransform& CurTrans, const FVector& TargetLoc,
	                    float InteropSpeed = 5.f);
#pragma endregion
};
