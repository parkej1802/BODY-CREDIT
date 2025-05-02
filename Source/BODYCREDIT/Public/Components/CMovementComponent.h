#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "Interfaces/IBindInput.h"
#include "CMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ESpeedType : uint8
{
	CROUCH = 0,
	RUN,
	SPRINT,
	MAX
};

UCLASS()
class BODYCREDIT_API UCMovementComponent
	: public UCBaseComponent
	, public IIBindInput
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	class UInputAction* IA_Movement;

	UPROPERTY(VisibleAnywhere)
	class UInputAction* IA_Look;

	UPROPERTY(VisibleAnywhere)
	class UInputAction* IA_Sprint;

	UPROPERTY(VisibleAnywhere)
	class UInputAction* IA_Crouch;

	UPROPERTY(VisibleAnywhere)
	class UInputAction* IA_Jump;

public:
	UCMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	virtual void BindInput(class UEnhancedInputComponent* InEnhancedInputComponent) override;

private:
	// Inputs
	void OnMoveForward(const struct FInputActionValue& InVal);
	void OnMoveRight(const struct FInputActionValue& InVal);

	void OnHorizontalLook(const struct FInputActionValue& InVal);
	void OnVerticalLook(const struct FInputActionValue& InVal);

	void OnSprint(const struct FInputActionValue& InVal);
	void OnReset(const struct FInputActionValue& InVal);
	
	void OnCrouch(const struct FInputActionValue& InVal);
	
	void OnJump(const struct FInputActionValue& InVal);

public:
	void SetSpeed(ESpeedType InType);

public:
	void SetCrouchSpeed();
	void SetRunSpeed();
	void SetSprintSpeed();

	void EnableControlRotation();
	void DisableControlRotation();

private:
	void Init();

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed[(int32)ESpeedType::MAX] = { 200, 600, 1000 };

private:
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float HorizontalLook = 20;

	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float VerticalLook = 20;

private:
	bool bCanMove = true;
	bool bFixedCamera = false;
	bool bCrouch = false;

	// FOV
	const float DefaultFOV = 90;
	const float SprintFOV = 110;
	const float FOVInterpSpeed = 5;
	float TargetFOV = DefaultFOV;

};
