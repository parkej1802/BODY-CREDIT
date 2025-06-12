#include "State/MEMORY/CCombat_BeamState_MEMORY.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"

void CCombat_BeamState_MEMORY::Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;
	AICon->StopMovement();
	
	ACNox_MemoryCollectorAI* MemoryCollector = Cast<ACNox_MemoryCollectorAI>(Owner);
	if (!bFired)
	{
		MemoryCollector->ShutBeam();
		bFired = true;
	}
	else
	{
		if (!MemoryCollector->IsPlayBeam())
		{
			Owner->UsingSkill(ESkillCoolDown::Beam);
			Owner->SetEnemyState(EEnemyState::Sense);
			bFired = false;
		}
	}
}
