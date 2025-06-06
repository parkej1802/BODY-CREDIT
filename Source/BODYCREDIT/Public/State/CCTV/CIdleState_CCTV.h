#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

/**
 * 
 */
class BODYCREDIT_API CIdleState_CCTV : public ICEnemyStateStrategy
{
private:
	TUniquePtr<class CRotateMoveStrategy> MoveStrategy;
	
public:
	CIdleState_CCTV(TUniquePtr<class CRotateMoveStrategy> InMoveStrategy);
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;
	virtual void ResetVal(class ACNox_EBase* Owner) override;
};
