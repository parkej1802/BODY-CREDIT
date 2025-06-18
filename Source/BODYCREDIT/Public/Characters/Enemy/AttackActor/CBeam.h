#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBeam.generated.h"

UCLASS()
class BODYCREDIT_API ACBeam : public AActor
{
	GENERATED_BODY()
	
public:	
	ACBeam();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	// 컴포넌트 생성 및 초기화
	void CreateComponents();
	void SetupVFXAssets();
	void DeactivateAllVFX();

	// 빔 업데이트 관련
	void UpdateDamageTimer(float DeltaTime);
	void UpdateBeamPosition(float DeltaTime);
	void UpdateAttackDelay(float DeltaTime);
	
	// 빔 계산 관련
	FVector CalculateBeamEnd(float DeltaTime);
	bool PerformLineTrace(const FVector& Start, const FVector& End, FHitResult& OutHitResult);
	
	// 빔 충돌 처리
	void HandleBeamHit(const FHitResult& HitResult);
	void HandleBeamMiss(const FVector& EndLocation);

	// 빔 활성화/비활성화
	void InitializeBeam(AActor* InTarget);
	void DeactivateBeam();

public:
	void SetBeamActive(bool bInActive, AActor* InTarget);

private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* rootScene;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* LaserBeamVFX;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* FireBallVFX;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* HitVFX;

	UPROPERTY()
	class ACNox_MemoryCollectorAI* OwnerAI;

	UPROPERTY()
	class AActor* TargetActor;

	FTimerHandle ActiveTimer;
	bool AttackStart = false;
	bool bApplyDamage = false;
	float DamageTimer = 0.0f;
	float DamageInterval = 0.3f;
	float CurPitch = 0.0f;
	float CurAttackDelay = 0.0f;
	float AttackDelay = 1.0f;
	float ActiveDelay = 0.5f;
	float AttackRange = 1000.0f;
}; 