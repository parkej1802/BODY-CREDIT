#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_API CCombat_DefaultState_MEDIC : public ICEnemyStateStrategy
{
private:
	bool bFired = false;
	float AcceptanceThreshold = 80.f;

public:
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;
};
