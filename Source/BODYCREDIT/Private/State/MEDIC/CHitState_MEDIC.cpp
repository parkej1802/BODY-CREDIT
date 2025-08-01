#include "State/MEDIC/CHitState_MEDIC.h"
#include "Characters/Enemy/CNox_EBase.h"

void CHitState_MEDIC::Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp)
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
