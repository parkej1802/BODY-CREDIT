#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CMoveStrategy.generated.h"

UINTERFACE(MinimalAPI)
class UCMoveStrategy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BODYCREDIT_API ICMoveStrategy
{
	GENERATED_BODY()

public:
	virtual void Move(class ACNox_EBase* Owner, float DeltaTime) = 0;
};
