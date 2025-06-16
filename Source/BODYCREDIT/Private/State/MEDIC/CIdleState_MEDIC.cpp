#include "State/MEDIC/CIdleState_MEDIC.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "State/MEDIC/CRandomMoveStrategy.h"

CIdleState_MEDIC::CIdleState_MEDIC(TUniquePtr<CRandomMoveStrategy> InMoveStrategy)
	: MoveStrategy(MoveTemp(InMoveStrategy))
{
}

void CIdleState_MEDIC::Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp)
{
	Owner->PlayIdleSound();
	
	if (MoveStrategy) MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());
}
