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
		// 이동하지 않았고, 공격 범위 내 플레이어가 없다면 플레이어를 향해 회전
		//Owner->RotateToTarget(Owner->GetWorld()->GetDeltaSeconds(), Owner->GetTransform(), Owner->GetTarget()->GetActorLocation());
		FRotator CurRot = Owner->GetActorRotation();
		float TargetYaw = (Owner->GetTarget()->GetActorLocation() - Owner->GetActorLocation()).Rotation().Yaw;
		FRotator TargetRot(CurRot.Pitch, TargetYaw, CurRot.Roll);
		Owner->SetActorRotation(TargetRot);
	}
	else
	{
		// Beam, PulseWave 중 스킬을 선택하고
		ECombatState ChooseSkill;
		if (ChooseRandomSkill(Owner, ChooseSkill))
		{
			Owner->SetCombatState(ChooseSkill);
			Owner->SetEnemyState(EEnemyState::Combat);
		}
		// 만약, 둘 중 하나도 선택되지 않았다면 기본 공격 조건을 체크한다.
		else if (Owner->IsSkillReady(ESkillCoolDown::Ranged))
		{
			// 기본 공격이 사용가능하면 기본 공격으로 상태 전환
			Owner->SetCombatState(ECombatState::Default);
			Owner->SetEnemyState(EEnemyState::Combat);
		}
		
		bIsMove = false;
	}	
}

bool CConditionalMoveStrategy_MEMORY::ChooseRandomSkill(ACNox_EBase* Owner, ECombatState& OutChooseSkill)
{
	// 1. 준비된 스킬 인덱스 수집
	TArray<int32> Ready;
	for (int32 i = 0; i < Skills.Num(); ++i)
	{
		if (Owner->IsSkillReady(Skills[i].GetSkill()))
		{
			Ready.Add(i);
		}
	}
	if (Ready.Num() == 0) return false; // 전부 쿨타임 중이면 아무것도 하지 않음

	// 2. 가중치 랜덤
	int32 SkillWeightSum = 0;
	const int32 rnd = FMath::RandRange(1, 100);
	for (const int32& idx : Ready)
	{
		SkillWeightSum += Skills[idx].GetWeight();
		if (rnd <= SkillWeightSum)
		{
			OutChooseSkill = Skills[idx].GetSkill() == ESkillCoolDown::Beam
				                 ? ECombatState::Beam
				                 : ECombatState::WavePulse;
			return true;
		}
	}

	return false;
}
