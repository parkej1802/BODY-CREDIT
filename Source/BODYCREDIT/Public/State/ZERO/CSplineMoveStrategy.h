#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Enemy/CMoveStrategy.h"

class ACPatrolRoute;
class ACNox_EBase;
/**
 * 
 */
class BODYCREDIT_API CSplineMoveStrategy : public ICMoveStrategy
{
private:
	ACPatrolRoute* Spline;
	TArray<float> Distances;       // 각 포인트 거리
	int32 CurrentIndex = 0;

	float AcceptanceRadius = 10.f;
	float AcceptanceThreshold = 50.f;
	bool bMoving = false;

public:
	CSplineMoveStrategy(ACPatrolRoute* InSpline);

	virtual void Move(ACNox_EBase* Owner, float DeltaTime) override;
};
