#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CEnemyStateStrategy.h"

class CRandomMoveStrategy_Memory;
/**
 * 
 */
class BODYCREDIT_API CIdleState_MEMORY : public ICEnemyStateStrategy
{
	TUniquePtr<CRandomMoveStrategy_Memory> MoveStrategy;

	bool HavingMemory(ACNox_EBase* Owner);
	bool MemoryCheck(ACNox_EBase* Owner);
	
public:
	CIdleState_MEMORY(TUniquePtr<CRandomMoveStrategy_Memory> InMoveStrategy);
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) override;
};
