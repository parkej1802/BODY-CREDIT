#include "State/CCTV/CDieState_CCTV.h"

#include "Characters/Enemy/CNox_EBase.h"

void CDieState_CCTV::Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp)
{
	if (!bFired)
	{
		Owner->HandleDie();
		bFired = true;
	}
}
