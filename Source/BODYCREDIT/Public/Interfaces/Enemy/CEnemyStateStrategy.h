#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CEnemyStateStrategy.generated.h"

UINTERFACE(MinimalAPI)
class UCEnemyStateStrategy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BODYCREDIT_API ICEnemyStateStrategy
{
	GENERATED_BODY()

public:
	virtual void Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp) = 0;
};
