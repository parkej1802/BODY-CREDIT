// Fill out your copyright notice in the Description page of Project Settings.


#include "BODYCREDIT/Public/Characters/Enemy/AI/Tasks/CBTT_MoveAlongPatrolRoute.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Navigation/PathFollowingComponent.h"
#include "Patrol/CPatrolRoute.h"
#include "Utilities/CLog.h"

namespace
{
	constexpr float DefaultAcceptanceRadius = 10.0f;
}

UCBTT_MoveAlongPatrolRoute::UCBTT_MoveAlongPatrolRoute()
{
	bCreateNodeInstance = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_MoveAlongPatrolRoute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerBT = &OwnerComp;
	auto PatrolRoute = GetPatrolRoute(OwnerComp);
	if (!PatrolRoute)
	{
		return EBTNodeResult::Failed;
	}

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(PatrolRoute->GetNextPointAsPosition());
	MoveRequest.SetAcceptanceRadius(DefaultAcceptanceRadius);

	auto AIController = OwnerComp.GetAIOwner();
	if (AIController)
	{
		// 이벤트 중복 방지를 위해 이전 핸들러 제거
		AIController->ReceiveMoveCompleted.RemoveDynamic(this, &UCBTT_MoveAlongPatrolRoute::OnPatrolResult);

		AIController->ReceiveMoveCompleted.AddDynamic(this, &UCBTT_MoveAlongPatrolRoute::OnPatrolResult);
		AIController->MoveTo(MoveRequest);
	}

	return EBTNodeResult::InProgress;
}

void UCBTT_MoveAlongPatrolRoute::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UCBTT_MoveAlongPatrolRoute::OnPatrolResult(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	// CLog::Log(FString::Printf(TEXT("PatrolRouteIndex: %d"), GetPatrolRoute(*OwnerBT)->PatrolRouteIndex));
	GetPatrolRoute(*OwnerBT)->IncrementPatrolRoute();
	FinishLatentTask(*OwnerBT, EBTNodeResult::Succeeded);
}

ACPatrolRoute* UCBTT_MoveAlongPatrolRoute::GetPatrolRoute(const UBehaviorTreeComponent& OwnerComp) const
{
	return Cast<ACPatrolRoute>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("PatrolRoute")));
}
