#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_API CDieState_MEMORY : public ICEnemyStateStrategy
{
private:
	bool bFired = false;

public:
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;
};
