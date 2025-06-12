#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_API CCombat_BeamState_MEMORY : public ICEnemyStateStrategy
{
private:
	bool bFired = false;

public:
	virtual void Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp) override;
};
