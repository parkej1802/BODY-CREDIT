#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "Interfaces/IBindInput.h"
#include "CMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ESpeedType : uint8
{
	STAND_WALK,			// Stand Walk
	STAND_RUN_FWD,		// W
	STAND_RUN_BWD,		// S
	STAND_RUN_RLWD,		// D or A
	CROUCH_WALK_FWD,	// Crouch W
	CROUCH_WALK_BWD,	// Crouch S
	CROUCH_WALK_RLWD,	// Crouch D or A
	SPRINT,			 // LShift 보류
	MAX,
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

	UPROPERTY(VisibleAnywhere) // 보류
	class UInputAction* IA_Sprint;

	UPROPERTY(VisibleAnywhere)
	class UInputAction* IA_Crouch;

	UPROPERTY(VisibleAnywhere)
	class UInputAction* IA_Walk;

	UPROPERTY(VisibleAnywhere)
	class UInputAction* IA_Jump;

public:
	FORCEINLINE bool CanMove() { return bCanMove; }
	FORCEINLINE bool IsForward() { return bForward; }
	FORCEINLINE bool IsSprint() { return bSprint; }
	FORCEINLINE bool IsCrouch() { return bCrouch; }

	// Stand
	FORCEINLINE float GetStandWalkSpeed() { return Speed[(int32)ESpeedType::STAND_WALK]; }
	FORCEINLINE float GetStandRunForwardSpeed() { return Speed[(int32)ESpeedType::STAND_RUN_FWD]; }
	FORCEINLINE float GetStandRunBackwardSpeed() { return Speed[(int32)ESpeedType::STAND_RUN_BWD]; }
	FORCEINLINE float GetStandRunRLwardSpeed() { return Speed[(int32)ESpeedType::STAND_RUN_RLWD]; }

	// Crouch
	FORCEINLINE float GetCrouchRunForwardSpeed() { return Speed[(int32)ESpeedType::CROUCH_WALK_FWD]; }
	FORCEINLINE float GetCrouchRunBackwardSpeed() { return Speed[(int32)ESpeedType::CROUCH_WALK_BWD]; }
	FORCEINLINE float GetCrouchRunRLwardSpeed() { return Speed[(int32)ESpeedType::CROUCH_WALK_RLWD]; }

	FORCEINLINE bool GetFixedCamera() { return bFixedCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }

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
	void OnMovement(const struct FInputActionValue& InVal);
	void OffMovement(const struct FInputActionValue& InVal);

	void OnHorizontalLook(const struct FInputActionValue& InVal);
	void OnVerticalLook(const struct FInputActionValue& InVal);

	void OnSprint(const struct FInputActionValue& InVal);
	void OnReset(const struct FInputActionValue& InVal);
	
	void OnCrouch(const struct FInputActionValue& InVal);
	
	void OnJump(const struct FInputActionValue& InVal);

public:
	void SetSpeed(ESpeedType InType);

public:
	void SetStandWalkSpeed();
	void SetStandRunForwardSpeed();
	void SetStandRunBackwardSpeed();
	void SetStandRunRLwardSpeed();

	void SetCrouchWalkForwardSpeed();
	void SetCrouchWalkBackwardSpeed();
	void SetCrouchWalkRLwardSpeed();

	void SetSprintSpeed();

	void EnableControlRotation();
	void DisableControlRotation();

public:
	void Move();
	void Stop();

private:
	void Init();

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed[(int32)ESpeedType::MAX] = { 350, 500, 300, 350, 300, 300, 300, 700 };

private:
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float HorizontalLook = 20;

	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float VerticalLook = 20;

private:
	bool bCanMove = true;
	bool bForward = false;
	bool bSprint = false;
	bool bCrouch = false;
	bool bFixedCamera = false;

	// FOV
	const float DefaultFOV = 90;
	const float RunFOV = 100;
	const float SprintFOV = 110;
	const float FOVInterpSpeed = 3;
	float TargetFOV = DefaultFOV;

};
