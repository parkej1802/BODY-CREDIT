#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_API CCombat_GrenadeState_MEDIC : public ICEnemyStateStrategy
{
private:
	bool bFired = false;
	const float AcceptanceThreshold = 30.f;

public:
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;
};
