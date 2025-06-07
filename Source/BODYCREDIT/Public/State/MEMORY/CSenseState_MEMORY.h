#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

class CConditionalMoveStrategy_MEMORY;
/**
 * 
 */
class BODYCREDIT_API CSenseState_MEMORY : public ICEnemyStateStrategy
{
private:
	TUniquePtr<CConditionalMoveStrategy_MEMORY> MoveStrategy;

public:
	CSenseState_MEMORY(TUniquePtr<CConditionalMoveStrategy_MEMORY> InMoveStrategy);
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;
};
