#include "State/MEDIC/CCombat_GrenadeState_MEDIC.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_MedicAndroid.h"
#include "Navigation/PathFollowingComponent.h"

void CCombat_GrenadeState_MEDIC::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;

	Owner->SetMovementSpeed(EEnemyMovementSpeed::Sprinting);
	EPathFollowingRequestResult::Type result = AICon->MoveToActor(Owner->GetTarget(), AcceptanceThreshold, true);
	if (result == EPathFollowingRequestResult::Failed)
	{
		AICon->StopMovement();
		return;
	}

	ACNox_MedicAndroid* Medic = Cast<ACNox_MedicAndroid>(Owner);
	if (!bFired)
	{
		bFired = true;
		Medic->HandleElectricGrenade();
	}
	else
	{
		if (!Medic->IsPlayingGrenade())
		{
			bFired = false;
			Owner->UsingSkill(ESkillCoolDown::Grenade);
			Owner->SetEnemyState(EEnemyState::Sense);
		}
	}
}
