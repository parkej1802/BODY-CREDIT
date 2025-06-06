#include "State/ZERO/CHitState_ZERO.h"

#include "Characters/Enemy/CNox_EBase.h"

void CHitState_ZERO::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	if (!bFired)
	{
		int32 rnd = FMath::RandRange(1, 4);
		Owner->HandleHit(rnd);
		bFired = true;
	}
	else
	{
		if (!Owner->IsHitting())
		{
			bFired = false;
			Owner->SetEnemyState(EEnemyState::Sense);
		}
	}
}
