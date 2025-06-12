#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

class CRandomMoveStrategy;
/**
 * 
 */
class BODYCREDIT_API CIdleState_MEDIC : public ICEnemyStateStrategy
{
	TUniquePtr<CRandomMoveStrategy> MoveStrategy;
	
public:
	CIdleState_MEDIC(TUniquePtr<CRandomMoveStrategy> InMoveStrategy);
	virtual void Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp) override;
};
