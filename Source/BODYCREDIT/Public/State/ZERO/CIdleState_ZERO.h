#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_API CIdleState_ZERO : public ICEnemyStateStrategy
{
private:
	TUniquePtr<class CSplineMoveStrategy> MoveStrategy;
	
public:
	CIdleState_ZERO(TUniquePtr<class CSplineMoveStrategy> InMoveStrategy);
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;
};
