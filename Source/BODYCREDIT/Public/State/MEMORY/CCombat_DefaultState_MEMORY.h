#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_API CCombat_DefaultState_MEMORY : public ICEnemyStateStrategy
{
private:
	bool bFired = false;
	float AcceptanceThreshold = 500.f;
	
public:
	virtual void Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp) override;
};
