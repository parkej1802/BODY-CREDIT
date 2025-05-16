#include "Characters/Enemy/AI/Services/CBTS_MedicHealthCheck.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_MedicAndroid.h"

void UCBTS_MedicHealthCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (auto* SelfActor = Cast<ACNox_MedicAndroid>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		SelfActor->SetHealFlag(SelfActor->IsLowHealth());
	}
}
