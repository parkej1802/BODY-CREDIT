#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CMoveStrategy.h"

enum class ECombatState : uint8;
/**
 * 
 */
class BODYCREDIT_API CRotateMoveStrategy : public ICMoveStrategy
{
public:
	virtual void Move(ACNox_EBase* Owner, float DeltaTime) override;
};
