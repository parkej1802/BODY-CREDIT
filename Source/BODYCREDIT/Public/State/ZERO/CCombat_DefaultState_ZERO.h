#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_API CCombat_DefaultState_ZERO : public ICEnemyStateStrategy
{
private:
	TUniquePtr<class CDefaultAttackStrategy_ZERO> AttackStrategy;
	
public:
	CCombat_DefaultState_ZERO(TUniquePtr<class CDefaultAttackStrategy_ZERO> InAttackStrategy);
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;
};
