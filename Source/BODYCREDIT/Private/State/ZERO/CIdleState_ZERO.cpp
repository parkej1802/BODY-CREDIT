#include "State/ZERO/CIdleState_ZERO.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "State/ZERO/CSplineMoveStrategy.h"

CIdleState_ZERO::CIdleState_ZERO(TUniquePtr<class CSplineMoveStrategy> InMoveStrategy): MoveStrategy(
	MoveTemp(InMoveStrategy))
{}

void CIdleState_ZERO::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	if (MoveStrategy)
	{
		MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());
	}
}
