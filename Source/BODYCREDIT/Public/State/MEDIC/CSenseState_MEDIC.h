#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

class CConditionalMoveStrategy_MEDIC;
/**
 * 
 */
class BODYCREDIT_API CSenseState_MEDIC : public ICEnemyStateStrategy
{
private:
	TUniquePtr<CConditionalMoveStrategy_MEDIC> MoveStrategy;

public:
	CSenseState_MEDIC(TUniquePtr<CConditionalMoveStrategy_MEDIC> InMoveStrategy);
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;
};
