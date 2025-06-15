#include "State/MEMORY/CConditionalMoveStrategy_MEMORY.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Navigation/PathFollowingComponent.h"

void CConditionalMoveStrategy_MEMORY::Move(ACNox_EBase* Owner, float DeltaTime)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;

	if (Skills.Num() == 0)
	{
		Skills.Emplace(ESkillCoolDown::Beam, 20.f);
		Skills.Emplace(ESkillCoolDown::WavePulse, 20.f);
		Skills.Emplace(ESkillCoolDown::Ranged);
	}

	// EQS로 전환 필요
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
	CovertToCombatState(Owner); // 공격 쿨타임이 지났으면 공격 상태로 전환

	// AcceptanceThreshold보다 멀면 EQS로 다가오고
	// 가까우면 MoveToActor로 이동
}

void CConditionalMoveStrategy_MEMORY::ResetVal(ACNox_EBase* Owner)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;
	AICon->StopMovement();
}

void CConditionalMoveStrategy_MEMORY::CovertToCombatState(ACNox_EBase* Owner)
{
	if (!bIsMove && !Owner->IsPlayerInForwardDegree(RangeAttackRange))
	{
		// 이동하지 않았고, 공격 범위 안에 플레이어가 없다면 플레이어를 향해 회전
		Owner->SetRotateToTarget();
	}
	else
	{
		TryCombatStateTransition(Owner);
		
		bIsMove = false;
	}	
}

void CConditionalMoveStrategy_MEMORY::TryCombatStateTransition(ACNox_EBase* Owner)
{
	ECombatState chosenSkill;
	if (TrySpecialSkillTransition(Owner, chosenSkill))
	{
		return;
	}
		
	// 기본 공격으로 전환 시도
	if (TryDefaultAttackTransition(Owner))
	{
		return;
	}
}

bool CConditionalMoveStrategy_MEMORY::TrySpecialSkillTransition(ACNox_EBase* Owner, ECombatState& OutChosenSkill)
{
	if (!ChooseRandomSkill(Owner, OutChosenSkill))
	{
		return false;
	}
		
	Owner->SetCombatState(OutChosenSkill);
	Owner->SetEnemyState(EEnemyState::Combat);
	return true;
}

bool CConditionalMoveStrategy_MEMORY::TryDefaultAttackTransition(ACNox_EBase* Owner)
{
	if (!Owner->IsSkillReady(ESkillCoolDown::Ranged))
	{
		return false;
	}
		
	Owner->SetCombatState(ECombatState::Default);
	Owner->SetEnemyState(EEnemyState::Combat);
	return true;
}

bool CConditionalMoveStrategy_MEMORY::ChooseRandomSkill(ACNox_EBase* Owner, ECombatState& OutChooseSkill)
{
	// 1. 준비된 스킬 수집
	TArray<int32> ReadySkillIndices;
	ReadySkillIndices.Reserve(Skills.Num()); // 메모리 최적화
    
	for (int32 i = 0; i < Skills.Num(); ++i)
	{
		if (Owner->IsSkillReady(Skills[i].GetSkill()))
		{
			ReadySkillIndices.Add(i);
		}
	}
    
	// 2. 사용 가능한 스킬이 없으면 종료
	if (ReadySkillIndices.IsEmpty())
	{
		return false;
	}
    
	// 3. 가중치 기반 랜덤 선택
	const int32 TotalWeight = CalculateTotalWeight(ReadySkillIndices);
	const int32 RandomValue = FMath::RandRange(1, TotalWeight);
    
	// 4. 가중치에 따른 스킬 선택
	int32 CurrentWeight = 0;
	for (const int32 SkillIndex : ReadySkillIndices)
	{
		CurrentWeight += Skills[SkillIndex].GetWeight();
		if (RandomValue <= CurrentWeight)
		{
			OutChooseSkill = ConvertSkillToCombatState(Skills[SkillIndex].GetSkill());
			return true;
		}
	}

	return false;
}

int32 CConditionalMoveStrategy_MEMORY::CalculateTotalWeight(const TArray<int32>& SkillIndices) const
{
	int32 TotalWeight = 0;
	for (const int32 Index : SkillIndices)
	{
		TotalWeight += Skills[Index].GetWeight();
	}
	return TotalWeight;
}

ECombatState CConditionalMoveStrategy_MEMORY::ConvertSkillToCombatState(ESkillCoolDown Skill) const
{
	return Skill == ESkillCoolDown::Beam ? ECombatState::Beam : ECombatState::WavePulse;
}
