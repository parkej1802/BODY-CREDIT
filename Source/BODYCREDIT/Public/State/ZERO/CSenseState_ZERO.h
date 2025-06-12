#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_API CSenseState_ZERO : public ICEnemyStateStrategy
{
private:
	TUniquePtr<class CConditionalMoveStrategy_ZERO> MoveStrategy;
	
public:
	CSenseState_ZERO(TUniquePtr<class CConditionalMoveStrategy_ZERO> InMoveStrategy);
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;
};
