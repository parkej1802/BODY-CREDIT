#include "State/MEMORY/CIdleState_MEMORY.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"
#include "State/MEMORY/CRandomMoveStrategy_Memory.h"

CIdleState_MEMORY::CIdleState_MEMORY(TUniquePtr<CRandomMoveStrategy_Memory> InMoveStrategy)
	: MoveStrategy(MoveTemp(InMoveStrategy))
{
}

void CIdleState_MEMORY::Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp)
{
	// 1. 기억평가 수행 (기억조각을 가지고 있는지 + 유효한 조각이 있는지)
	if (HavingMemory(Owner) && MemoryCheck(Owner))
	{
		// 2. 유효한 기억이 있다면 MemoryHunt 상태로 전이
		Owner->SetEnemyState(EEnemyState::MemoryHunt);
		return;
	}
	// 3. 유효한 기억이 없다면 랜덤 이동 수행
	if (MoveStrategy) MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());
}

bool CIdleState_MEMORY::HavingMemory(ACNox_EBase* Owner)
{
	ACNox_MemoryCollectorAI* MemoryCollector = Cast<ACNox_MemoryCollectorAI>(Owner);
	return MemoryCollector->IsMemoryEmpty();
}

bool CIdleState_MEMORY::MemoryCheck(ACNox_EBase* Owner)
{
	ACNox_MemoryCollectorAI* MemoryCollector = Cast<ACNox_MemoryCollectorAI>(Owner);
	return MemoryCollector->EvaluateMemory();
}
