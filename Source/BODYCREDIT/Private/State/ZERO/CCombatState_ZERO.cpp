#include "State/ZERO/CCombatState_ZERO.h"

#include "Components/Enemy/CFSMComponent.h"
#include "State/ZERO/CCombat_DefaultState_ZERO.h"

CCombatState_ZERO::CCombatState_ZERO()
{
	CombatSubStrategies.Add(ECombatState::Default, MakeShared<CCombat_DefaultState_ZERO>());
}

void CCombatState_ZERO::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	ECombatState SubState = FSMComp->GetCombatState();

	if (CombatSubStrategies.Contains(SubState))
	{
		CombatSubStrategies[SubState]->Execute(Owner, FSMComp);
	}
}
