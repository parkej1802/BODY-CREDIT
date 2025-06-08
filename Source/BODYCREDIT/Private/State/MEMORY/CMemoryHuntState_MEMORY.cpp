#include "State/MEMORY/CMemoryHuntState_MEMORY.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "State/MEMORY/CMemoryMoveStrategy.h"

CMemoryHuntState_MEMORY::CMemoryHuntState_MEMORY(TUniquePtr<CMemoryMoveStrategy> InMoveStrategy)
	: MoveStrategy(MoveTemp(InMoveStrategy))
{}

void CMemoryHuntState_MEMORY::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	if (MoveStrategy) MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());
}
