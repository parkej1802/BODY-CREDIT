#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

enum class ECombatState : uint8;
/**
 * 
 */
class BODYCREDIT_API CCombatState_MEMORY : public ICEnemyStateStrategy
{
public:
	CCombatState_MEMORY();
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;

private:
	TMap<ECombatState, TSharedPtr<ICEnemyStateStrategy>> CombatSubStrategies;
};
