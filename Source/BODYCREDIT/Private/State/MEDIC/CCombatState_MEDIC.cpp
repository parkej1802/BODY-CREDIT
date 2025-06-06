#include "State/MEDIC/CCombatState_MEDIC.h"

#include "Components/Enemy/CFSMComponent.h"
#include "State/MEDIC/CCombat_DefaultState_MEDIC.h"
#include "State/MEDIC/CCombat_GrenadeState_MEDIC.h"
#include "State/MEDIC/CCombat_HealState_MEDIC.h"

CCombatState_MEDIC::CCombatState_MEDIC()
{
	CombatSubStrategies.Add(ECombatState::Default, MakeShared<CCombat_DefaultState_MEDIC>());
	CombatSubStrategies.Add(ECombatState::Grenade, MakeShared<CCombat_GrenadeState_MEDIC>());
	CombatSubStrategies.Add(ECombatState::Heal, MakeShared<CCombat_HealState_MEDIC>());
}

void CCombatState_MEDIC::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	ECombatState SubState = FSMComp->GetCombatState();

	if (CombatSubStrategies.Contains(SubState))
	{
		CombatSubStrategies[SubState]->Execute(Owner, FSMComp);
	}
}
