#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CMoveStrategy.h"

class ACNox_EBase;
/**
 * 
 */
class BODYCREDIT_API CRandomMoveStrategy_Memory : public ICMoveStrategy
{
private:
	float RandomRadius = 1000;
	float AcceptanceThreshold = 10.f;
	bool bMoving = false;
	FVector RanLocation = FVector::ZeroVector;
	
	FVector LastPosition = FVector::ZeroVector; // 마지막 위치
	float StuckTime = 0.f; // 한 자리에 머무른 시간
	
	void RandomMove(ACNox_EBase* Owner);
	FVector GetRandomLocation(const ACNox_EBase* Owner) const;
	
public:
	virtual void Move(ACNox_EBase* Owner, float DeltaTime) override;
};
