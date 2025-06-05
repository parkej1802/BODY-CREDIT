#include "State/ZERO/CConditionalMoveStrategy_ZERO.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Navigation/PathFollowingComponent.h"

void CConditionalMoveStrategy_ZERO::Move(ACNox_EBase* Owner, float DeltaTime)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;

	Owner->SetMovementSpeed(EEnemyMovementSpeed::Sprinting);
	EPathFollowingRequestResult::Type result = AICon->MoveToActor(Owner->GetTarget(), AcceptanceThreshold, true);
	switch (result)
	{
	case EPathFollowingRequestResult::Failed:
		break;
	case EPathFollowingRequestResult::AlreadyAtGoal:
		CovertToCombatState(Owner); // 공격 쿨타임이 지났으면 공격 상태로 전환
		break;
	default:
		break;
	}

	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Melee, DeltaTime);
}

void CConditionalMoveStrategy_ZERO::CovertToCombatState(ACNox_EBase* Owner)
{
	if (Owner->IsSkillReady(ESkillCoolDown::Melee))
	{
		Owner->SetCombatState(ECombatState::Default);
		Owner->SetEnemyState(EEnemyState::Combat);
	}
}
