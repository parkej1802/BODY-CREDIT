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
	ACPatrolRoute* Spline = nullptr;
	TArray<float> Distances; // 각 포인트 거리
	int32 CurrentIndex = 0;

	float AcceptanceRadius = 10.f;
	float AcceptanceThreshold = 50.f;
	bool bMoving = false;

	FVector LastPosition = FVector::ZeroVector; // 마지막 위치
	float StuckTime = 0.f; // 한 자리에 머무른 시간

	float RandomRadius = 400;
	FVector RanLocation;

	void SplineMove(ACNox_EBase* Owner);
	void RandomMove(ACNox_EBase* Owner);
	void HearingMove(ACNox_EBase* Owner);
	FVector GetRandomLocation(const ACNox_EBase* Owner) const;

public:
	CSplineMoveStrategy(ACPatrolRoute* InSpline);
	virtual void Move(ACNox_EBase* Owner, float DeltaTime) override;
	virtual void ResetVal(ACNox_EBase* Owner) override;
};
