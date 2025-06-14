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
	case EPathFollowingRequestResult::AlreadyAtGoal:
		break;
	default:
		bIsMove = true;
		break;
	}

	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Melee, DeltaTime);
	CovertToCombatState(Owner); // 공격 쿨타임이 지났으면 공격 상태로 전환
}

void CConditionalMoveStrategy_ZERO::ResetVal(ACNox_EBase* Owner)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;
	AICon->StopMovement();
}

void CConditionalMoveStrategy_ZERO::CovertToCombatState(ACNox_EBase* Owner)
{
	if (!bIsMove && !Owner->IsPlayerInForwardDegree(MeleeAttackRange))
	{
		// 이동하지 않았고, 공격 범위 안에 플레이어가 없다면 플레이어를 향해 회전
		Owner->SetRotateToTarget();
	}
	else
	{
		if (Owner->IsSkillReady(ESkillCoolDown::Melee))
		{
			Owner->SetCombatState(ECombatState::Default);
			Owner->SetEnemyState(EEnemyState::Combat);
		}
		bIsMove = false;
	}	
}
