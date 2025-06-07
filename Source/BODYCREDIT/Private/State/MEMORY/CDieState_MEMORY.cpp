#include "State/MEMORY/CDieState_MEMORY.h"

#include "Characters/Enemy/CNox_EBase.h"

void CDieState_MEMORY::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	if (!bFired)
	{
		Owner->HandleDie();
		bFired = true;
	}
}
