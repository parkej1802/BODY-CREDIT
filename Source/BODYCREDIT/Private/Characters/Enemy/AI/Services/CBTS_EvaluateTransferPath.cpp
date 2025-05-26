// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Services/CBTS_EvaluateTransferPath.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"
#include "Data/CMemoryData.h"
#include "Transportation/CStair.h"
#include "Transportation/CVent.h"

void UCBTS_EvaluateTransferPath::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	struct FPathOption
	{
		float Score;
		FVector EntryLocation;
		FVector ExitLocation;
		EMovementPathType Type;
	};

	TArray<FPathOption> Candidates;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return;

	auto* SelfActor = Cast<ACNox_MemoryCollectorAI>(OwnerComp.GetAIOwner()->GetPawn());
	auto MemoryTarget = SelfActor->GetMemoryTarget();
	auto AllVents = SelfActor->GetAllVent();
	auto AllStairs = SelfActor->GetAllStair();
	
	FVector AILocation = SelfActor->GetActorLocation();
	
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
		float Score = FVector::Dist(AILocation, BestEntry->GetActorLocation()) + FVector::Dist(BestExit->GetActorLocation(), MemoryTarget.Location);
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
	
		Blackboard->SetValueAsEnum(MovementTypeKey.SelectedKeyName, static_cast<uint8>(Candidates[0].Type));
		Blackboard->SetValueAsVector(MovementTargetLocationKey.SelectedKeyName, Candidates[0].EntryLocation);
		Blackboard->SetValueAsVector(NextTargetAfterTransferKey.SelectedKeyName, Candidates[0].ExitLocation);
	}
}
