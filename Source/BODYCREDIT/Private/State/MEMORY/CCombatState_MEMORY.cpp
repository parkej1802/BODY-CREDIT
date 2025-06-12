#include "State/MEMORY/CCombatState_MEMORY.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "Components/Enemy/CFSMComponent.h"
#include "State/MEMORY/CCombat_BeamState_MEMORY.h"
#include "State/MEMORY/CCombat_DefaultState_MEMORY.h"
#include "State/MEMORY/CCombat_WavePulseState_MEMORY.h"

CCombatState_MEMORY::CCombatState_MEMORY()
{
	CombatSubStrategies.Add(ECombatState::Default, MakeShared<CCombat_DefaultState_MEMORY>());
	CombatSubStrategies.Add(ECombatState::Beam, MakeShared<CCombat_BeamState_MEMORY>());
	CombatSubStrategies.Add(ECombatState::WavePulse, MakeShared<CCombat_WavePulseState_MEMORY>());
}

void CCombatState_MEMORY::Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp)
{
	ECombatState SubState = FSMComp->GetCombatState();

	if (CombatSubStrategies.Contains(SubState))
	{
		CombatSubStrategies[SubState]->Execute(Owner, FSMComp);
	}

	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Ranged, Owner->GetWorld()->GetDeltaSeconds());
	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Beam, Owner->GetWorld()->GetDeltaSeconds());
	Owner->UpdateSkillCoolDowns(ESkillCoolDown::WavePulse, Owner->GetWorld()->GetDeltaSeconds());
}
