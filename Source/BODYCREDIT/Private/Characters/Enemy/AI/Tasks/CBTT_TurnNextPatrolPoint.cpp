// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/AI/Tasks/CBTT_TurnNextPatrolPoint.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Characters/Enemy/CNox_Zero.h"
#include "Kismet/KismetMathLibrary.h"
#include "Patrol/CPatrolRoute.h"

UCBTT_TurnNextPatrolPoint::UCBTT_TurnNextPatrolPoint()
{
	bCreateNodeInstance = true;
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTT_TurnNextPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AICon = OwnerComp.GetAIOwner();
	APawn* Pawn = AICon ? AICon->GetPawn() : nullptr;
	if (!Pawn)
		return EBTNodeResult::Failed;

	FVector MyLocation = Pawn->GetActorLocation();

	auto PatrolRoute = Cast<ACPatrolRoute>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("PatrolRoute")));
	FVector PatrolTargetLocation = PatrolRoute->GetNextPointAsPosition();
	PatrolTargetLocation.Z = MyLocation.Z;

	FBTTask_RotateMemory* MyMemory = (FBTTask_RotateMemory*)NodeMemory;
	MyMemory->TargetRotation = UKismetMathLibrary::FindLookAtRotation(MyLocation, PatrolTargetLocation);
	MyMemory->bRotationSet = true;

	auto* Enemy = Cast<ACNox_Zero>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor")));
	// Enemy->SetDesiredRotation(MyMemory->TargetRotation);

	return EBTNodeResult::InProgress;
}

void UCBTT_TurnNextPatrolPoint::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("Target")))
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	
	auto* Enemy = Cast<ACNox_Zero>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor")));
	FBTTask_RotateMemory* MyMemory = (FBTTask_RotateMemory*)NodeMemory;

	FRotator CurrentRot = Enemy->GetActorRotation();
	FRotator TargetRot = MyMemory->TargetRotation;

	// 보간 회전
	// FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, DeltaSeconds, RotationSpeed / 90.0f);
	FRotator NewRot = FMath::RInterpConstantTo(CurrentRot, TargetRot, DeltaSeconds, RotationSpeed);
	Enemy->SetActorRotation(NewRot);

	float DeltaYaw = FMath::Abs(FMath::FindDeltaAngleDegrees(CurrentRot.Yaw, TargetRot.Yaw));
	if (DeltaYaw < AcceptableAngle)
	{
		// Enemy->SetDesiredRotation(FRotator::ZeroRotator);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
