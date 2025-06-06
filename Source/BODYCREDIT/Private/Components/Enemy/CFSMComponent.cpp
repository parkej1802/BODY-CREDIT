#include "Components/Enemy/CFSMComponent.h"
#include "../../../Public/Interfaces/Enemy/CEnemyStateStrategy.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_Zero.h"
#include "State/CCTV/CDieState_CCTV.h"
#include "State/CCTV/CIdleState_CCTV.h"
#include "State/CCTV/CRotateMoveStrategy.h"
#include "State/MEDIC/CCombatState_MEDIC.h"
#include "State/MEDIC/CConditionalMoveStrategy_MEDIC.h"
#include "State/MEDIC/CDieState_MEDIC.h"
#include "State/MEDIC/CIdleState_MEDIC.h"
#include "State/MEDIC/CRandomMoveStrategy.h"
#include "State/MEDIC/CSenseState_MEDIC.h"
#include "State/MEMORY/CCombatState_MEMORY.h"
#include "State/MEMORY/CDieState_MEMORY.h"
#include "State/MEMORY/CIdleState_MEMORY.h"
#include "State/MEMORY/CMemoryHuntState_MEMORY.h"
#include "State/MEMORY/CSenseState_MEMORY.h"
#include "State/ZERO/CCombatState_ZERO.h"
#include "State/ZERO/CConditionalMoveStrategy_ZERO.h"
#include "State/ZERO/CDieState_ZERO.h"
#include "State/ZERO/CHitState_ZERO.h"
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
	InitSkillCoolDowns(OwnerEnemy->GetEnemyType());
}

TMap<EEnemyState, TSharedPtr<ICEnemyStateStrategy>> UCFSMComponent::CreateStrategies(EEnemyType Type)
{
	TMap<EEnemyState, TSharedPtr<ICEnemyStateStrategy>> Result;

	switch (Type)
	{
	case EEnemyType::Cctv:
		{
			{
				TUniquePtr<CRotateMoveStrategy> MoveStrategy = MakeUnique<CRotateMoveStrategy>();
				Result.Add(EEnemyState::IDLE, MakeShared<CIdleState_CCTV>(MoveTemp(MoveStrategy)));
			}
			Result.Add(EEnemyState::Die, MakeShared<CDieState_CCTV>());
		}
		break;
	case EEnemyType::Zero:
		{
			{
				TUniquePtr<CSplineMoveStrategy> MoveStrategy = MakeUnique<CSplineMoveStrategy>(
					Cast<ACNox_Zero>(OwnerEnemy)->GetNearPatrolRoute());
				Result.Add(EEnemyState::IDLE, MakeShared<CIdleState_ZERO>(MoveTemp(MoveStrategy)));
			}
			{
				TUniquePtr<CConditionalMoveStrategy_ZERO> ConditionalMove = MakeUnique<CConditionalMoveStrategy_ZERO>();
				Result.Add(EEnemyState::Sense, MakeShared<CSenseState_ZERO>(MoveTemp(ConditionalMove)));
			}
			Result.Add(EEnemyState::Hit, MakeShared<CHitState_ZERO>());
			Result.Add(EEnemyState::Combat, MakeShared<CCombatState_ZERO>());
			Result.Add(EEnemyState::Die, MakeShared<CDieState_ZERO>());
		}
		break;
	case EEnemyType::MedicAndroid:
		{
			TUniquePtr<CRandomMoveStrategy> MoveStrategy = MakeUnique<CRandomMoveStrategy>();
			Result.Add(EEnemyState::IDLE, MakeShared<CIdleState_MEDIC>(MoveTemp(MoveStrategy)));
		}
		{
			TUniquePtr<CConditionalMoveStrategy_MEDIC> ConditionalMove = MakeUnique<CConditionalMoveStrategy_MEDIC>();
			Result.Add(EEnemyState::Sense, MakeShared<CSenseState_MEDIC>(MoveTemp(ConditionalMove)));
		}
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

void UCFSMComponent::ResetVal(EEnemyType Type)
{
	switch (Type)
	{
	case EEnemyType::Zero:
		if (CurrentEnemyState == EEnemyState::Hit)
		{
			EnemyStrategies[EEnemyState::IDLE]->ResetVal(OwnerEnemy);
			EnemyStrategies[EEnemyState::Sense]->ResetVal(OwnerEnemy);
			EnemyStrategies[EEnemyState::Combat]->ResetVal(OwnerEnemy);
		}
		else if (CurrentEnemyState == EEnemyState::Combat)
		{
			EnemyStrategies[EEnemyState::IDLE]->ResetVal(OwnerEnemy);
			EnemyStrategies[EEnemyState::Sense]->ResetVal(OwnerEnemy);
		}
		else if (CurrentEnemyState == EEnemyState::Sense)
		{
			EnemyStrategies[EEnemyState::IDLE]->ResetVal(OwnerEnemy);
		}
		break;
	case EEnemyType::MedicAndroid:
		EnemyStrategies[EEnemyState::Sense]->ResetVal(OwnerEnemy);
	}
}

void UCFSMComponent::SetEnemyState(EEnemyState NewState)
{
	CurrentEnemyState = NewState;
}

void UCFSMComponent::SetCombatState(ECombatState NewCombatState)
{
	CurrentCombatState = NewCombatState;
}

void UCFSMComponent::InitSkillCoolDowns(EEnemyType Type)
{
	switch (Type)
	{
	case EEnemyType::Zero:
		SkillCoolDowns.Add(GetSkillName(ESkillCoolDown::Melee), 0.f);
		SkillMaxCoolDowns.Add(GetSkillName(ESkillCoolDown::Melee), 1.f);
		break;
	case EEnemyType::MedicAndroid:
		SkillCoolDowns.Add(GetSkillName(ESkillCoolDown::Melee), 0.f);
		SkillMaxCoolDowns.Add(GetSkillName(ESkillCoolDown::Melee), 1.f);
		SkillCoolDowns.Add(GetSkillName(ESkillCoolDown::Heal), 0.f);
		SkillMaxCoolDowns.Add(GetSkillName(ESkillCoolDown::Heal), 20.f);
		SkillCoolDowns.Add(GetSkillName(ESkillCoolDown::Grenade), 0.f);
		SkillMaxCoolDowns.Add(GetSkillName(ESkillCoolDown::Grenade), 10.f);
		break;
	default:
		break;
	}
}

FName UCFSMComponent::GetSkillName(ESkillCoolDown SkillType) const
{
	switch (SkillType)
	{
	case ESkillCoolDown::Melee: return FName(TEXT("MeleeCoolDown"));
	case ESkillCoolDown::Ranged: return FName(TEXT("RangedCoolDown"));
	case ESkillCoolDown::Heal: return FName(TEXT("HealCoolDown"));
	case ESkillCoolDown::Grenade: return FName(TEXT("GrenadeCoolDown"));
	default: return FName(TEXT("UnknownCoolDown"));
	}
}
