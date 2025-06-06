#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_API CHitState_MEDIC : public ICEnemyStateStrategy
{	
public:
	virtual void Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp) override;
	
private:
	bool bFired = false;
};
