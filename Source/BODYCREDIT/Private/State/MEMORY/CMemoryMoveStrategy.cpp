#include "State/MEMORY/CMemoryMoveStrategy.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"
#include "Navigation/PathFollowingComponent.h"
#include "Transportation/CStair.h"
#include "Transportation/CVent.h"

void CMemoryMoveStrategy::Move(ACNox_EBase* Owner, float DeltaTime)
{
	if (!Owner) return;

	switch (CurMoveState)
	{
	case None: // 아직 이동 수단을 선택하지 않음
		// 1. 기억 조각의 위치가 같은 층인지 다른 층인지 확인
		if (IsOtherFloor(Owner))
		{
			// 2. 이동 수단 선택
		}
		break;
	case MoveToStairState: // 계단으로 이동
		break;
	case MoveToVentState: // 벤트로 이동
		break;
	case MoveToMemoryState: // 기억 조각으로 이동(같은 층일때)
		MoveToMemory(Owner);
		break;
	default:
		break;
	}
}

bool CMemoryMoveStrategy::IsOtherFloor(ACNox_EBase* Owner)
{
	ACNox_MemoryCollectorAI* MemoryCollector = Cast<ACNox_MemoryCollectorAI>(Owner);
	if (!MemoryCollector)
	{
		EndStrategy(Owner);
		return false;
	}

	bool res = FMath::Abs(MemoryCollector->GetActorLocation().Z - MemoryCollector->GetMemoryTarget().Location.Z) >
		100.f;
	if (!res) CurMoveState = MoveToMemoryState;
	return res;
}

void CMemoryMoveStrategy::EvaluateTransferPath(ACNox_EBase* Owner)
{
	struct FPathOption
	{
		float Score;
		FVector EntryLocation;
		FVector ExitLocation;
		EMovementPathType Type;
	};

	TArray<FPathOption> Candidates;

	auto* MemoryCollector = Cast<ACNox_MemoryCollectorAI>(Owner);
	auto MemoryTarget = MemoryCollector->GetMemoryTarget();
	auto AllVents = MemoryCollector->GetAllVent();
	auto AllStairs = MemoryCollector->GetAllStair();

	FVector AILocation = MemoryCollector->GetActorLocation();

	// 밴트 후보
	ACVent* BestEntry = nullptr;
	ACVent* BestExit = nullptr;
	float MinEntryDist = TNumericLimits<float>::Max();
	float MinExitDist = TNumericLimits<float>::Max();
	// 밴트 후보 검색
	for (auto* Vent : AllVents)
	{
		float EntryDist = FVector::Dist(AILocation, Vent->GetActorLocation());
		float ExitDist = FVector::Dist(MemoryTarget.Location, Vent->GetActorLocation());

		if (EntryDist < MinEntryDist)
		{
			MinEntryDist = EntryDist;
			BestEntry = Vent;
		}

		if (ExitDist < MinExitDist)
		{
			MinExitDist = ExitDist;
			BestExit = Vent;
		}
	}

	if (BestEntry && BestExit && BestEntry != BestExit)
	{
		float Score = FVector::Dist(AILocation, BestEntry->GetActorLocation()) + FVector::Dist(
			BestExit->GetActorLocation(), MemoryTarget.Location);
		Candidates.Add({Score, BestEntry->GetActorLocation(), BestExit->GetActorLocation(), EMovementPathType::Vent});
	}
	//============================

	// 계단 후보 검색
	for (auto* Stair : AllStairs)
	{
		FVector Entry, Exit;
		Stair->GetClosestEntryAndExit(AILocation, Entry, Exit);

		float Score = FVector::Dist(AILocation, Entry) + FVector::Dist(Exit, MemoryTarget.Location);
		Candidates.Add({Score, Entry, Exit, EMovementPathType::Stairs});
	}

	if (Candidates.Num() > 0)
	{
		Candidates.Sort([](const FPathOption& A, const FPathOption& B)
		{
			return A.Score < B.Score;
		});

		Candidates[0].Type == EMovementPathType::Vent
			? CurMoveState = MoveToVentState
			: CurMoveState = MoveToStairState;
		MovementTargetLoc = Candidates[0].EntryLocation;
		NextTargetAfterTransfer = Candidates[0].ExitLocation;
	}
}

void CMemoryMoveStrategy::MoveToStair(ACNox_EBase* Owner)
{
	if (!bMoving)
	{
		if (MoveToLoc(Owner, MovementTargetLoc)) bMoving = true;
	}
	else
	{
		if (MoveToLoc(Owner, NextTargetAfterTransfer))
		{
			bMoving = false;
			CurMoveState = MoveToMemoryState;
		}
	}
}

void CMemoryMoveStrategy::MoveToVent(ACNox_EBase* Owner)
{
	if (!bMoving)
	{
		if (MoveToLoc(Owner, MovementTargetLoc)) bMoving = true;
	}
	else
	{
		if (MoveToLoc(Owner, NextTargetAfterTransfer))
		{
			bMoving = false;
			CurMoveState = MoveToMemoryState;
		}
	}
}

void CMemoryMoveStrategy::MoveToMemory(ACNox_EBase* Owner)
{
	ACNox_MemoryCollectorAI* MemoryCollector = Cast<ACNox_MemoryCollectorAI>(Owner);
	if (MoveToLoc(Owner, MemoryCollector->GetMemoryTarget().Location))
		EndStrategy(Owner);
}

bool CMemoryMoveStrategy::MoveToLoc(ACNox_EBase* Owner, const FVector TargetLoc)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon)
	{
		EndStrategy(Owner);
		return false;
	}

	// 이동속도 변경
	Owner->SetMovementSpeed(EEnemyMovementSpeed::Walking);
	switch (AICon->MoveToLocation(TargetLoc, AcceptanceThreshold, true))
	{
	case EPathFollowingRequestResult::Failed:
	case EPathFollowingRequestResult::AlreadyAtGoal:
		return true;
	default:
		return false;
	}
}

void CMemoryMoveStrategy::EndStrategy(ACNox_EBase* Owner)
{
	ACNox_MemoryCollectorAI* MemoryCollector = Cast<ACNox_MemoryCollectorAI>(Owner);
	MemoryCollector->SetMemoryTarget_MemoryMoveEnd(FMemoryFragment());
	CurMoveState = None;
	bMoving = false;
	Owner->SetEnemyState(EEnemyState::IDLE);
}
