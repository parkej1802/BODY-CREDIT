#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTT_WavePulse.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_WavePulse : public UBTTaskNode
{
	GENERATED_BODY()

private:
	UCBTT_WavePulse();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
