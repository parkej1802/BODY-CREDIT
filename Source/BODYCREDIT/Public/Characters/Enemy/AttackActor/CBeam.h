// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBeam.generated.h"

UCLASS()
class BODYCREDIT_API ACBeam : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACBeam();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ACNox_MemoryCollectorAI* OwnerAI;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	void SetBeamActive(bool bInActive, AActor* InTarget = nullptr);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	class USceneComponent* rootScene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	class UNiagaraComponent* LaserBeamVFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	class UNiagaraComponent* FireBallVFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	class UNiagaraComponent* HitVFX;

	UPROPERTY()
	UMaterialInstance* FireDecal;
	UPROPERTY()
	UMaterialInstance* GlowDecal;

	FTimerHandle DecalTimer;
	UPROPERTY(EditDefaultsOnly)
	float DecalDelay = 0.2f;

	bool AttackStart = false;

private:
	UPROPERTY()
	AActor* TargetActor;

	FTimerHandle ActiveTimer;
	UPROPERTY(EditDefaultsOnly)
	float ActiveDelay = 1.5f;

	UPROPERTY(EditDefaultsOnly)
	float AttackRange = 3000.f;

	UPROPERTY(EditDefaultsOnly)
	float AttackDelay = .5f;
	float CurAttackDelay = 0.f;

	float CurPitch = 0.f;
};
