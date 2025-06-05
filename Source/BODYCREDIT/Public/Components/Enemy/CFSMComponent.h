#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Enemy/CEnemyState.h"
#include "Global.h"
#include "CFSMComponent.generated.h"

class ACNox_EBase;
class ICEnemyStateStrategy;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BODYCREDIT_API UCFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCFSMComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	ACNox_EBase* OwnerEnemy = nullptr;
	EEnemyState CurrentEnemyState = EEnemyState::IDLE;
	ECombatState CurrentCombatState = ECombatState::Default;

	TMap<EEnemyState, TSharedPtr<ICEnemyStateStrategy>> EnemyStrategies;

	TMap<EEnemyState, TSharedPtr<ICEnemyStateStrategy>> CreateStrategies(EEnemyType Type);

public:
	void InitializeFSM(ACNox_EBase* InOwner);
	void UpdateState();
	void ResetVal(EEnemyType Type);

	EEnemyState GetEnemyState() const { return CurrentEnemyState; }
	ECombatState GetCombatState() const { return CurrentCombatState; }

	void SetEnemyState(EEnemyState NewState);
	void SetCombatState(ECombatState NewCombatState);

private:
	TMap<FName, float> SkillCoolDowns;
	TMap<FName, float> SkillMaxCoolDowns;
	void InitSkillCoolDowns(EEnemyType Type);

public:
	FName GetSkillName(ESkillCoolDown SkillType) const;
	
	void UpdateSkillCoolDowns(ESkillCoolDown Skill, float DeltaTime)
	{
		SkillCoolDowns[GetSkillName(Skill)] += DeltaTime;
		CLog::Log(FString::Printf(TEXT("SkillCoolDowns[%s] = %f"), *GetSkillName(Skill).ToString(), SkillCoolDowns[GetSkillName(Skill)]));		
	}

	bool IsSkillReady(ESkillCoolDown Skill) const
	{
		return SkillCoolDowns[GetSkillName(Skill)] >= SkillMaxCoolDowns[GetSkillName(Skill)];
	}

	void UsingSkill(ESkillCoolDown Skill)
	{
		SkillCoolDowns[GetSkillName(Skill)] = 0.f;
	}
};
