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
#include "State/MEDIC/CHitState_MEDIC.h"
#include "State/MEDIC/CIdleState_MEDIC.h"
#include "State/MEDIC/CRandomMoveStrategy.h"
#include "State/MEDIC/CSenseState_MEDIC.h"
#include "State/MEMORY/CCombatState_MEMORY.h"
#include "State/MEMORY/CConditionalMoveStrategy_MEMORY.h"
#include "State/MEMORY/CDieState_MEMORY.h"
#include "State/MEMORY/CHitState_MEMORY.h"
#include "State/MEMORY/CIdleState_MEMORY.h"
#include "State/MEMORY/CMemoryHuntState_MEMORY.h"
#include "State/MEMORY/CRandomMoveStrategy_Memory.h"
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

void UCFSMComponent::UpdateState()
{
	if (EnemyStrategies.Contains(CurrentEnemyState)) EnemyStrategies[CurrentEnemyState]->Execute(OwnerEnemy, this);
}

#pragma region Init State
void UCFSMComponent::InitializeFSM(ACNox_EBase* InOwner)
{
	OwnerEnemy = InOwner;
	EnemyStrategies = CreateStrategies(OwnerEnemy->GetEnemyType());
	InitSkillCoolDowns(OwnerEnemy->GetEnemyType());
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
		SkillCoolDowns.Add(GetSkillName(ESkillCoolDown::Ranged), 0.f);
		SkillMaxCoolDowns.Add(GetSkillName(ESkillCoolDown::Ranged), 1.f);
		SkillCoolDowns.Add(GetSkillName(ESkillCoolDown::Beam), 0.f);
		SkillMaxCoolDowns.Add(GetSkillName(ESkillCoolDown::Beam), 20.f);
		SkillCoolDowns.Add(GetSkillName(ESkillCoolDown::WavePulse), 0.f);
		SkillMaxCoolDowns.Add(GetSkillName(ESkillCoolDown::WavePulse), 20.f);
		break;
	}
}
#pragma endregion

#pragma region Create Strategies
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
			{
				TUniquePtr<CRandomMoveStrategy> MoveStrategy = MakeUnique<CRandomMoveStrategy>();
				Result.Add(EEnemyState::IDLE, MakeShared<CIdleState_MEDIC>(MoveTemp(MoveStrategy)));
			}
			{
				TUniquePtr<CConditionalMoveStrategy_MEDIC> ConditionalMove = MakeUnique<
					CConditionalMoveStrategy_MEDIC>();
				Result.Add(EEnemyState::Sense, MakeShared<CSenseState_MEDIC>(MoveTemp(ConditionalMove)));
			}
			Result.Add(EEnemyState::Hit, MakeShared<CHitState_MEDIC>());
			Result.Add(EEnemyState::Combat, MakeShared<CCombatState_MEDIC>());
			Result.Add(EEnemyState::Die, MakeShared<CDieState_MEDIC>());
		}
		break;
	case EEnemyType::MemoryCollector:
		{
			{
				TUniquePtr<CRandomMoveStrategy_Memory> MoveStrategy = MakeUnique<CRandomMoveStrategy_Memory>();
				Result.Add(EEnemyState::IDLE, MakeShared<CIdleState_MEMORY>(MoveTemp(MoveStrategy)));
			}
			{
				TUniquePtr<CConditionalMoveStrategy_MEMORY> ConditionalMove = MakeUnique<CConditionalMoveStrategy_MEMORY>();
				Result.Add(EEnemyState::Sense, MakeShared<CSenseState_MEMORY>(MoveTemp(ConditionalMove)));
			}
			Result.Add(EEnemyState::MemoryHunt, MakeShared<CMemoryHuntState_MEMORY>());
			Result.Add(EEnemyState::Combat, MakeShared<CCombatState_MEMORY>());
			Result.Add(EEnemyState::Hit, MakeShared<CHitState_MEMORY>());
			Result.Add(EEnemyState::Die, MakeShared<CDieState_MEMORY>());
		}		
		break;
	}
	return Result;
}
#pragma endregion

#pragma region Reset Value
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
	case EEnemyType::MemoryCollector:
		EnemyStrategies[EEnemyState::Sense]->ResetVal(OwnerEnemy);
		break;
	}
}
#pragma endregion

#pragma region Set State
void UCFSMComponent::SetEnemyState(EEnemyState NewState)
{
	CurrentEnemyState = NewState;
}

void UCFSMComponent::SetCombatState(ECombatState NewCombatState)
{
	CurrentCombatState = NewCombatState;
}
#pragma endregion

#pragma region Get Skill CoolDown Name
FName UCFSMComponent::GetSkillName(ESkillCoolDown SkillType) const
{
	switch (SkillType)
	{
	case ESkillCoolDown::Melee: return FName(TEXT("MeleeCoolDown"));
	case ESkillCoolDown::Ranged: return FName(TEXT("RangedCoolDown"));
	case ESkillCoolDown::Heal: return FName(TEXT("HealCoolDown"));
	case ESkillCoolDown::Grenade: return FName(TEXT("GrenadeCoolDown"));
	case ESkillCoolDown::Beam: return FName(TEXT("BeamCoolDown"));
	case ESkillCoolDown::WavePulse: return FName(TEXT("WavePulseCoolDown"));
	default: return FName(TEXT("UnknownCoolDown"));
	}
}
#pragma endregion
