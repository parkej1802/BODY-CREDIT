#include "Characters/Enemy/AI/Services/CBTS_TargetInDegree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Global.h"

UE_DISABLE_OPTIMIZATION
void UCBTS_TargetInDegree::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AActor* Enemy = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("SelfActor")));
	AActor* PlayerActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));

	FVector MyLocation = Enemy->GetActorLocation();
	FVector Forward = Enemy->GetActorForwardVector();
	FVector ToPlayer = (PlayerActor->GetActorLocation() - MyLocation).GetSafeNormal();

	float range = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(RangeKey.SelectedKeyName);
	if (FVector::Dist(MyLocation, PlayerActor->GetActorLocation()) > range)
	{
		CLog::Log("Out Of Range");
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(ConditionKey.SelectedKeyName, false);
		return;
	}
	
	float Dot = FVector::DotProduct(Forward, ToPlayer);
	// CLog::Log(FString::Printf(TEXT("Dot: %f, Degree: %f"), Dot, Degree));
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(ConditionKey.SelectedKeyName, Dot <= Degree);
}
UE_ENABLE_OPTIMIZATION 