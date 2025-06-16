#include "State/ZERO/CIdleState_ZERO.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "State/ZERO/CSplineMoveStrategy.h"

CIdleState_ZERO::CIdleState_ZERO(TUniquePtr<class CSplineMoveStrategy> InMoveStrategy): MoveStrategy(
	MoveTemp(InMoveStrategy))
{}

void CIdleState_ZERO::Execute(ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	Owner->PlayIdleSound(FMath::RandRange(0, 1));
	
	if (MoveStrategy) MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());
}

void CIdleState_ZERO::ResetVal(ACNox_EBase* Owner)
{	
	if (MoveStrategy) MoveStrategy->ResetVal(Owner);
}
