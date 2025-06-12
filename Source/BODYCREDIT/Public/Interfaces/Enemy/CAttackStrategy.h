#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CAttackStrategy.generated.h"

class ACNox_EBase;
UINTERFACE(MinimalAPI)
class UCAttackStrategy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BODYCREDIT_API ICAttackStrategy
{
	GENERATED_BODY()

public:
	virtual void Execute(ACNox_EBase* Owner, class UCFSMComponent* FSMComp) = 0;
	virtual void ResetVal(ACNox_EBase* Owner) {};
};
