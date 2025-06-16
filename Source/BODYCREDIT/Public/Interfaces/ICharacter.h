#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ICharacter.generated.h"

UINTERFACE(MinimalAPI)
class UICharacter : public UInterface
{
	GENERATED_BODY()
};

class BODYCREDIT_API IICharacter
{
	GENERATED_BODY()

public:
	virtual void End_Avoid() {}
	virtual void End_Hitted() {}
	virtual void End_Dead() {}
	
};
