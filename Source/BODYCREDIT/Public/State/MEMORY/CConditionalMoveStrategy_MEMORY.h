#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CMoveStrategy.h"

enum class ESkillCoolDown;
enum class ECombatState : uint8;
class ACNox_EBase;

struct FEnemySkill
{
private:
	float SkillWeight;
	ESkillCoolDown Skill;

public:
	FEnemySkill(ESkillCoolDown InSkill, float InWeight = 0.f): Skill(InSkill), SkillWeight(InWeight)
	{ }

	ESkillCoolDown GetSkill() const { return Skill; }
	float GetWeight() const { return SkillWeight; }
};

/**
 * 
 */
class BODYCREDIT_API CConditionalMoveStrategy_MEMORY : public ICMoveStrategy
{
private:
	bool bIsMove = false;
	float AcceptanceThreshold = 800.f;
    const float RangeAttackRange = 800.f;

	TArray<FEnemySkill> Skills;

	void CovertToCombatState(ACNox_EBase* Owner);
	bool ChooseRandomSkill(ACNox_EBase* Owner, ECombatState& OutChooseSkill);

public:
	virtual void Move(ACNox_EBase* Owner, float DeltaTime) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;
};
