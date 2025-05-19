#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "Interfaces/IBindInput.h"
#include "CMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ESpeedType : uint8
{
	CROUCH = 0,		 // C (슬라이딩, 대쉬와 버튼 공유 예정)
	WALK,			 // LCtrl
	MOVE_FWD,		 // W
	MOVE_BWD,		 // S
	MOVE_RLWD,		 // D or A
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

	//UPROPERTY(VisibleAnywhere) // 보류
	//class UInputAction* IA_Sprint;

	UPROPERTY(VisibleAnywhere)
	class UInputAction* IA_Crouch;

	UPROPERTY(VisibleAnywhere)
	class UInputAction* IA_Walk;

	UPROPERTY(VisibleAnywhere)
	class UInputAction* IA_Jump;

public:
	FORCEINLINE bool CanMove() { return bCanMove; }

	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ESpeedType::WALK]; }
	//FORCEINLINE float GetRunSpeed() { return Speed[(int32)ESpeedType::Run]; }
	//FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ESpeedType::Sprint]; }

	FORCEINLINE bool GetFixedCamera() { return bFixedCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }

public:
	UCMovementComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	virtual void BindInput(class UEnhancedInputComponent* InEnhancedInputComponent) override;

private:
	// Inputs
	void OnMoveForward(const struct FInputActionValue& InVal);
	void OnMoveRight(const struct FInputActionValue& InVal);
	void OffMovement(const struct FInputActionValue& InVal);

	void OnHorizontalLook(const struct FInputActionValue& InVal);
	void OnVerticalLook(const struct FInputActionValue& InVal);

	void OnSprint(const struct FInputActionValue& InVal);
	void OnReset(const struct FInputActionValue& InVal);
	
	void OnCrouch(const struct FInputActionValue& InVal);
	
	void OnJump(const struct FInputActionValue& InVal);

public:
	void SetSpeed(ESpeedType InType);

	UFUNCTION(Reliable, Server)
	void ServerRPC_SetSpeed(ESpeedType InType);
	void ServerRPC_SetSpeed_Implementation(ESpeedType InType);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPC_SetSpeed(ESpeedType InType);
	void MulticastRPC_SetSpeed_Implementation(ESpeedType InType);

public:
	void UpdateSpeed();

	int32 IsPressed(ESpeedType InType);

	void SetCrouchSpeed();
	void SetWalkSpeed();
	void SetMoveForwardSpeed();
	void SetMoveBackwardSpeed();
	void SetMoveRLSpeed();
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
	float Speed[(int32)ESpeedType::MAX] = { 200, 400, 500, 300, 350, 600 };

	UPROPERTY(EditAnywhere, Replicated, Category = "Speed")
	int32 Pressed[(int32)ESpeedType::MAX];

private:
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float HorizontalLook = 20;

	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float VerticalLook = 20;

private:
	bool bCanMove = true;
	bool bFixedCamera = false;

	bool bMoveForward = false;

	// FOV
	const float DefaultFOV = 90;
	const float RunFOV = 100;
	const float SprintFOV = 110;
	const float FOVInterpSpeed = 3;
	float TargetFOV = DefaultFOV;

};
