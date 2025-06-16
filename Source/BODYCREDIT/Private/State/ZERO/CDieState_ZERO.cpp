#include "State/ZERO/CDieState_ZERO.h"

#include "Characters/Enemy/CNox_EBase.h"

void CDieState_ZERO::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	if (!bFired)
	{
		Owner->HandleDie(FMath::RandRange(1, 4));
		bFired = true;
	}
}
