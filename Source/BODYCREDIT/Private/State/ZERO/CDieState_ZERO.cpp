#include "State/ZERO/CDieState_ZERO.h"

#include "Characters/Enemy/CNox_EBase.h"

void CDieState_ZERO::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	if (!bFired)
	{
		int32 rnd = FMath::RandRange(1, 4);
		Owner->HandleDie(rnd);
		bFired = true;
	}
}
