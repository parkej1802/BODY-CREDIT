#include "State/ZERO/CDefaultAttackStrategy_ZERO.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Navigation/PathFollowingComponent.h"

void CDefaultAttackStrategy_ZERO::Execute(ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
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
	
	if (!bFired)
	{
		Owner->HandleAttack();
		bFired = true;
	}
	else
	{
		if (!Owner->IsAttacking())
		{
			Owner->UsingSkill(ESkillCoolDown::Melee);
			Owner->SetEnemyState(EEnemyState::Sense);
			bFired = false;
		}
	}
}

void CDefaultAttackStrategy_ZERO::ResetVal(ACNox_EBase* Owner)
{
	bFired = false;
}
