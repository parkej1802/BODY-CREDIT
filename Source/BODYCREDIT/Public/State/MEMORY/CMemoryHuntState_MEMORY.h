#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

class CMemoryMoveStrategy;
/**
 * 
 */
class BODYCREDIT_API CMemoryHuntState_MEMORY : public ICEnemyStateStrategy
{
private:
	TUniquePtr<CMemoryMoveStrategy> MoveStrategy;

public:
	CMemoryHuntState_MEMORY(TUniquePtr<CMemoryMoveStrategy> InMoveStrategy);
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;
};
