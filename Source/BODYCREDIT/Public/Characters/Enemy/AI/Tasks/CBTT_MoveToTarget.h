#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "CBTT_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class BODYCREDIT_API UCBTT_MoveToTarget : public UBTTask_MoveTo
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DirectionRadiusKey;
};
