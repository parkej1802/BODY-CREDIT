#include "State/ZERO/CSenseState_ZERO.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "State/ZERO/CConditionalMoveStrategy_ZERO.h"

CSenseState_ZERO::CSenseState_ZERO(TUniquePtr<CConditionalMoveStrategy_ZERO> InMoveStrategy)
	: MoveStrategy(MoveTemp(InMoveStrategy))
{
}

void CSenseState_ZERO::Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp)
{
	Owner->PlaySenseSound();
	if (MoveStrategy) MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());
}

void CSenseState_ZERO::ResetVal(ACNox_EBase* Owner)
{
	if (MoveStrategy) MoveStrategy->ResetVal(Owner);
}
