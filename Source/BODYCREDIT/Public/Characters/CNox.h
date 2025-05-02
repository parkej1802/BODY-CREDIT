#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CNox.generated.h"

UCLASS()
class BODYCREDIT_API ACNox : public ACharacter
{
	GENERATED_BODY()

public:
	ACNox();

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamID = 1;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

/**
 *	Status Property - LHJ (2025.05.02)
 *	추후 컴포넌트로 옮길 예정
 *	정수형이랑 이야기 함
 */
protected:
	UPROPERTY()
	float Health;

	UPROPERTY()
	float Defense;

/**
 *	Common Virtual Function - LHJ (2025.05.02)
 *	추후 컴포넌트로 옮길 예정
 */
protected:
	virtual void TakeDamage(float Amount);
	virtual void Die();
};
