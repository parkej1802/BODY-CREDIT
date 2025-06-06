#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CAttackStrategy.h"

class ACPatrolRoute;
class ACNox_EBase;
/**
 * 
 */
class BODYCREDIT_API CDefaultAttackStrategy_ZERO : public ICAttackStrategy
{
public:
	virtual void Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;

private:
	bool bFired = false;
	float AcceptanceThreshold = 30.f;
};
