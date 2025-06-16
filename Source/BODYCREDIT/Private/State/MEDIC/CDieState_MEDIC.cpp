#include "State/MEDIC/CDieState_MEDIC.h"

#include "Characters/Enemy/CNox_EBase.h"

void CDieState_MEDIC::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	if (!bFired)
	{
		Owner->HandleDie(FMath::RandRange(1, 4));
		Owner->PlayDieSound();
		bFired = true;
	}
}
