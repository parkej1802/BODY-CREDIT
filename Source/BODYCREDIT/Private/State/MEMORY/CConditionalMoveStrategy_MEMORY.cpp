

#include "State/MEMORY/CConditionalMoveStrategy_MEMORY.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_MedicAndroid.h"

void CConditionalMoveStrategy_MEMORY::Move(ACNox_EBase* Owner, float DeltaTime)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;

	Owner->SetMovementSpeed(EEnemyMovementSpeed::Sprinting);
	AICon->MoveToActor(Owner->GetTarget(), AcceptanceThreshold, true);
	CovertToCombatState(Owner); // 공격 쿨타임이 지났으면 공격 상태로 전환
}

void CConditionalMoveStrategy_MEMORY::ResetVal(ACNox_EBase* Owner)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;
	AICon->StopMovement();
}

void CConditionalMoveStrategy_MEMORY::CovertToCombatState(ACNox_EBase* Owner)
{
	
}
