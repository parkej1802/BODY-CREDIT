#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IBindInput.generated.h"

UINTERFACE(MinimalAPI)
class UIBindInput : public UInterface
{
	GENERATED_BODY()

};

class BODYCREDIT_API IIBindInput
{
	GENERATED_BODY()

public:
	virtual void BindInput(class UEnhancedInputComponent* InEnhancedInputComponent) = 0;

};
