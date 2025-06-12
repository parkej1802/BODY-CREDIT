#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

enum class ECombatState : uint8;
/**
 * 
 */
class BODYCREDIT_API CCombatState_MEDIC : public ICEnemyStateStrategy
{
public:
	CCombatState_MEDIC();
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;

private:
	TMap<ECombatState, TSharedPtr<ICEnemyStateStrategy>> CombatSubStrategies;
};
