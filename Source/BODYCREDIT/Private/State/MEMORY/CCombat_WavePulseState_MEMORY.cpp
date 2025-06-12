#include "State/MEMORY/CCombat_WavePulseState_MEMORY.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"

void CCombat_WavePulseState_MEMORY::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;
	AICon->StopMovement();
	
	ACNox_MemoryCollectorAI* MemoryCollector = Cast<ACNox_MemoryCollectorAI>(Owner);
	if (!bFired)
	{
		MemoryCollector->ShutPulseWave();
		bFired = true;
	}
	else
	{
		if (!MemoryCollector->IsPlayPulseWave())
		{
			Owner->UsingSkill(ESkillCoolDown::WavePulse);
			Owner->SetEnemyState(EEnemyState::Sense);
			bFired = false;
		}
	}
}
