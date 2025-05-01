#pragma once

#include "CoreMinimal.h"
#include "Characters/CNox.h"
#include "CNox_Runner.generated.h"

UCLASS()
class BODYCREDIT_API ACNox_Runner : public ACNox
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* Camera;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCMovementComponent* Movement;

private:
	UPROPERTY(VisibleAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* MappingContext;
	
public:
	ACNox_Runner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Init();

};
