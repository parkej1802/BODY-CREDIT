#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_API CSenseState_ZERO : public ICEnemyStateStrategy
{
private:
	TUniquePtr<class CConditionalMoveStrategy> MoveStrategy;
	
public:
	CSenseState_ZERO(TUniquePtr<class CConditionalMoveStrategy> InMoveStrategy);
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;
};
