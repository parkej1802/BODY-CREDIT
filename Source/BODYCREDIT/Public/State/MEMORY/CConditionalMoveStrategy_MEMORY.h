#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CMoveStrategy.h"

class ACNox_EBase;
/**
 * 
 */
class BODYCREDIT_API CConditionalMoveStrategy_MEMORY : public ICMoveStrategy
{
private:
    // const float MeleeAttackRange = 200.f;
    // const float GrenadeRange = 1500.f;

	// float AcceptanceRadius = 10.f;
	float AcceptanceThreshold = 500.f;

	void CovertToCombatState(ACNox_EBase* Owner);

public:
	virtual void Move(ACNox_EBase* Owner, float DeltaTime) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;
};
