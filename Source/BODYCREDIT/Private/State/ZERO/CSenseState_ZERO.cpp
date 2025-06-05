#include "State/ZERO/CSenseState_ZERO.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "State/ZERO/CConditionalMoveStrategy.h"

CSenseState_ZERO::CSenseState_ZERO(TUniquePtr<CConditionalMoveStrategy> InMoveStrategy)
	: MoveStrategy(MoveTemp(InMoveStrategy))
{
}

void CSenseState_ZERO::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	if (MoveStrategy)
	{
		MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());
	}
}
