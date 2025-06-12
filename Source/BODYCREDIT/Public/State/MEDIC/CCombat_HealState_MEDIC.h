#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_API CCombat_HealState_MEDIC : public ICEnemyStateStrategy
{
private:
	bool bFired = false;
	bool bHealEnd = false;
	float HealTime = 2.0f;
	float CurHealTime = 0.0f;
	
public:
	virtual void Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp) override;
};
