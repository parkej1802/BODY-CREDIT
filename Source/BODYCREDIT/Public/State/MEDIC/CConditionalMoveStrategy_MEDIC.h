#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CMoveStrategy.h"

class ACNox_EBase;
/**
 * 
 */
class BODYCREDIT_API CConditionalMoveStrategy_MEDIC : public ICMoveStrategy
{
private:
    const float MeleeAttackRange = 200.f;
    const float GrenadeRange = 1500.f;

	float AcceptanceRadius = 10.f;
	float AcceptanceThreshold = 40.f;
	bool bMoving = false;

	void CovertToCombatState(ACNox_EBase* Owner);

public:
	virtual void Move(ACNox_EBase* Owner, float DeltaTime) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;
};
