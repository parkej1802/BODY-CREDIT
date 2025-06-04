#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_API CIdleState_ZERO : public ICEnemyStateStrategy
{
public:
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;
};
