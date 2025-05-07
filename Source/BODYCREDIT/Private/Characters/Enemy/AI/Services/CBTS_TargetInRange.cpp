// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Services/CBTS_TargetInRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UCBTS_TargetInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return;
	
	AActor* SelfActor = OwnerComp.GetAIOwner()->GetPawn();
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	float MinAttackRange = Blackboard->GetValueAsFloat(MinRangeKey.SelectedKeyName);
	float MaxAttackRange = Blackboard->GetValueAsFloat(MaxRangeKey.SelectedKeyName);
	
	if (SelfActor && TargetActor)
	{
		float Distance = FVector::Dist(SelfActor->GetActorLocation(), TargetActor->GetActorLocation());
		bool bInRange = MinAttackRange < Distance && Distance <= MaxAttackRange;

		Blackboard->SetValueAsBool(IsInAttackRangeKey.SelectedKeyName, bInRange);
	}
	else
	{
		Blackboard->SetValueAsBool(IsInAttackRangeKey.SelectedKeyName, false);
	}
}
