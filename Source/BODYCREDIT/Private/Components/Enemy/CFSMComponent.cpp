#include "Components/Enemy/CFSMComponent.h"
#include "../../../Public/Interfaces/Enemy/CEnemyStateStrategy.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_Zero.h"
#include "State/CCTV/CDieState_CCTV.h"
#include "State/CCTV/CIdleState_CCTV.h"
#include "State/CCTV/CSenseState_CCTV.h"
#include "State/MEDIC/CCombatState_MEDIC.h"
#include "State/MEDIC/CDieState_MEDIC.h"
#include "State/MEDIC/CIdleState_MEDIC.h"
#include "State/MEDIC/CSenseState_MEDIC.h"
#include "State/MEMORY/CCombatState_MEMORY.h"
#include "State/MEMORY/CDieState_MEMORY.h"
#include "State/MEMORY/CIdleState_MEMORY.h"
#include "State/MEMORY/CMemoryHuntState_MEMORY.h"
#include "State/MEMORY/CSenseState_MEMORY.h"
#include "State/ZERO/CCombatState_ZERO.h"
#include "State/ZERO/CDieState_ZERO.h"
#include "State/ZERO/CIdleState_ZERO.h"
#include "State/ZERO/CSenseState_ZERO.h"
#include "State/ZERO/CSplineMoveStrategy.h"

UCFSMComponent::UCFSMComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCFSMComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCFSMComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCFSMComponent::InitializeFSM(ACNox_EBase* InOwner)
{
	OwnerEnemy = InOwner;
	EnemyStrategies = CreateStrategies(OwnerEnemy->GetEnemyType());
}

TMap<EEnemyState, TSharedPtr<ICEnemyStateStrategy>> UCFSMComponent::CreateStrategies(EEnemyType Type)
{
	TMap<EEnemyState, TSharedPtr<ICEnemyStateStrategy>> Result;

	switch (Type)
	{
	case EEnemyType::Cctv:
		Result.Add(EEnemyState::IDLE, MakeShared<CIdleState_CCTV>());
		Result.Add(EEnemyState::Sense, MakeShared<CSenseState_CCTV>());
		Result.Add(EEnemyState::Die, MakeShared<CDieState_CCTV>());
		break;
	case EEnemyType::Zero:
		{
			TUniquePtr<CSplineMoveStrategy> MoveStrategy = MakeUnique<CSplineMoveStrategy>(Cast<ACNox_Zero>(OwnerEnemy)->GetNearPatrolRoute());
			Result.Add(EEnemyState::IDLE, MakeShared<CIdleState_ZERO>(MoveTemp(MoveStrategy)));
		}
		Result.Add(EEnemyState::Sense, MakeShared<CSenseState_ZERO>());
		Result.Add(EEnemyState::Combat, MakeShared<CCombatState_ZERO>());
		Result.Add(EEnemyState::Die, MakeShared<CDieState_ZERO>());
		break;
	case EEnemyType::MedicAndroid:
		Result.Add(EEnemyState::IDLE, MakeShared<CIdleState_MEDIC>());
		Result.Add(EEnemyState::Sense, MakeShared<CSenseState_MEDIC>());
		Result.Add(EEnemyState::Combat, MakeShared<CCombatState_MEDIC>());
		Result.Add(EEnemyState::Die, MakeShared<CDieState_MEDIC>());
		break;
	case EEnemyType::MemoryCollector:
		Result.Add(EEnemyState::IDLE, MakeShared<CIdleState_MEMORY>());
		Result.Add(EEnemyState::Sense, MakeShared<CSenseState_MEMORY>());
		Result.Add(EEnemyState::MemoryHunt, MakeShared<CMemoryHuntState_MEMORY>());
		Result.Add(EEnemyState::Combat, MakeShared<CCombatState_MEMORY>());
		Result.Add(EEnemyState::Die, MakeShared<CDieState_MEMORY>());
		break;
	}
	return Result;
}

void UCFSMComponent::UpdateState()
{
	if (EnemyStrategies.Contains(CurrentEnemyState)) EnemyStrategies[CurrentEnemyState]->Execute(OwnerEnemy, this);
}

void UCFSMComponent::SetEnemyState(EEnemyState NewState)
{
	CurrentEnemyState = NewState;
}

void UCFSMComponent::SetCombatState(ECombatState NewCombatState)
{
	CurrentCombatState = NewCombatState;
}
