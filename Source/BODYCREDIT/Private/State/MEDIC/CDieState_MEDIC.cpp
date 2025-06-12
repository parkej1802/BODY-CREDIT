#include "State/MEDIC/CDieState_MEDIC.h"

#include "Characters/Enemy/CNox_EBase.h"

void CDieState_MEDIC::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	if (!bFired)
	{
		Owner->HandleDie();
		bFired = true;
	}
}
