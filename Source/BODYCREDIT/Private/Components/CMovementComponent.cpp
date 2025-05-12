#include "Components/CMovementComponent.h"
#include "Global.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Characters/CNox.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

UCMovementComponent::UCMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Init();

}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateSpeed();

	if (UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter))
		camera->SetFieldOfView(FMath::FInterpTo(camera->FieldOfView, TargetFOV, DeltaTime, FOVInterpSpeed));

}

void UCMovementComponent::BindInput(UEnhancedInputComponent* InEnhancedInputComponent)
{
	// Movement
	InEnhancedInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &UCMovementComponent::OnMoveForward);
	InEnhancedInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &UCMovementComponent::OnMoveRight);
	InEnhancedInputComponent->BindAction(IA_Movement, ETriggerEvent::Completed, this, &UCMovementComponent::OffMovement);

	// Look
	InEnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UCMovementComponent::OnHorizontalLook);
	InEnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UCMovementComponent::OnVerticalLook);

	//// Sprint // 보류
	//InEnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Started, this, &UCMovementComponent::OnSprint);
	//InEnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &UCMovementComponent::OnReset);

	// Crouch
	InEnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &UCMovementComponent::OnCrouch);

	// Jump
	InEnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &UCMovementComponent::OnJump);

}

void UCMovementComponent::OnMoveForward(const FInputActionValue& InVal)
{
	CheckFalse(bCanMove);

	TargetFOV = RunFOV;

	const FVector2D input = InVal.Get<FVector2D>();

	// 이동 방향 계산 (카메라 전방 기준)
	const float yaw = OwnerCharacter->GetControlRotation().Yaw;
	const FVector direction = FQuat(FRotator(0, yaw, 0)).GetForwardVector();

	if (input.X > 0) // Forward
	{
		Pressed[(uint8)ESpeedType::MOVE_FWD] = true;
		Pressed[(uint8)ESpeedType::MOVE_BWD] = false;
	}
	else if (input.X < 0) // Backward
	{
		Pressed[(uint8)ESpeedType::MOVE_BWD] = true;
		Pressed[(uint8)ESpeedType::MOVE_FWD] = false;
	}
	else
	{
		Pressed[(uint8)ESpeedType::MOVE_BWD] = false;
		Pressed[(uint8)ESpeedType::MOVE_FWD] = false;
	}

	// 이동 입력 적용
	OwnerCharacter->AddMovementInput(direction, input.X);

}

void UCMovementComponent::OnMoveRight(const FInputActionValue& InVal)
{
	CheckFalse(bCanMove);

	Pressed[(uint8)ESpeedType::MOVE_RLWD] = true;

	const FVector2D input = InVal.Get<FVector2D>();
	if (FMath::IsNearlyEqual(input.Y, 0))
		Pressed[(uint8)ESpeedType::MOVE_RLWD] = false;

	// 이동 방향 계산 (카메라 전방 기준)
	const float yaw = OwnerCharacter->GetControlRotation().Yaw;
	const FVector direction = FQuat(FRotator(0, yaw, 0)).GetRightVector();

	// 이동 입력 적용
	OwnerCharacter->AddMovementInput(direction, input.Y);

}

void UCMovementComponent::OffMovement(const FInputActionValue& InVal)
{
	TargetFOV = DefaultFOV;

	// Movement false 처리
	Pressed[(uint8)ESpeedType::MOVE_FWD] = false;
	Pressed[(uint8)ESpeedType::MOVE_BWD] = false;
	Pressed[(uint8)ESpeedType::MOVE_RLWD] = false;

}

void UCMovementComponent::OnHorizontalLook(const FInputActionValue& InVal)
{
	CheckTrue(bFixedCamera);

	const FVector2D input = InVal.Get<FVector2D>();
	CheckTrue(input.X == 0);

	OwnerCharacter->AddControllerYawInput(input.X * HorizontalLook * GetWorld()->GetDeltaSeconds());

}

void UCMovementComponent::OnVerticalLook(const FInputActionValue& InVal)
{
	CheckTrue(bFixedCamera);

	const FVector2D input = InVal.Get<FVector2D>();
	CheckTrue(input.Y == 0);

	OwnerCharacter->AddControllerPitchInput(input.Y * VerticalLook * GetWorld()->GetDeltaSeconds());

}

void UCMovementComponent::OnSprint(const FInputActionValue& InVal)
{
	SetSprintSpeed();

}

void UCMovementComponent::OnReset(const FInputActionValue& InVal)
{
	SetMoveForwardSpeed();

}

void UCMovementComponent::OnCrouch(const FInputActionValue& InVal)
{
	//if (OwnerCharacter->GetCharacterMovement()->IsFalling())
	//{
	//	const FVector launch = OwnerCharacter->GetActorForwardVector() * 500.f;
	//	OwnerCharacter->LaunchCharacter(launch, false, false);

	//	return;
	//}

	//SetCrouchState(!bCrouch);

}

void UCMovementComponent::OnJump(const FInputActionValue& InVal)
{
	OwnerCharacter->Jump();

}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	if (OwnerCharacter->IsLocallyControlled())
	{
		ServerRPC_SetSpeed(InType);

		return;
	}

	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(uint8)InType];

}

void UCMovementComponent::ServerRPC_SetSpeed_Implementation(ESpeedType InType)
{
	MulticastRPC_SetSpeed(InType);

}

void UCMovementComponent::MulticastRPC_SetSpeed_Implementation(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(uint8)InType];

}

void UCMovementComponent::UpdateSpeed()
{
	// FWD
	if (IsPressed(ESpeedType::MOVE_FWD))
	{
		SetMoveForwardSpeed();

		return;
	}

	// BWD
	else if (IsPressed(ESpeedType::MOVE_BWD))
	{
		SetMoveBackwardSpeed();

		return;
	}

	// RWD or LWD
	else if (IsPressed(ESpeedType::MOVE_RLWD))
	{
		SetMoveRLSpeed();

		return;
	}

}

int32 UCMovementComponent::IsPressed(ESpeedType InType)
{
	return Pressed[(int32)InType];

}

void UCMovementComponent::SetCrouchSpeed()
{
	SetSpeed(ESpeedType::CROUCH);

}

void UCMovementComponent::SetWalkSpeed()
{
	SetSpeed(ESpeedType::WALK);

}

void UCMovementComponent::SetMoveForwardSpeed()
{
	SetSpeed(ESpeedType::MOVE_FWD);

}

void UCMovementComponent::SetMoveBackwardSpeed()
{
	SetSpeed(ESpeedType::MOVE_BWD);

}

void UCMovementComponent::SetMoveRLSpeed()
{
	SetSpeed(ESpeedType::MOVE_RLWD);

}

void UCMovementComponent::SetSprintSpeed()
{
	SetSpeed(ESpeedType::SPRINT);

}

void UCMovementComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;

}

void UCMovementComponent::DisableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;

}

void UCMovementComponent::Move()
{
	bCanMove = true;

}

void UCMovementComponent::Stop()
{
	bCanMove = false;

}

void UCMovementComponent::Init()
{
	// Movement
	CHelpers::GetAsset<UInputAction>(&IA_Movement, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Movement.IA_Movement'"));

	// Look
	CHelpers::GetAsset<UInputAction>(&IA_Look, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Look.IA_Look'"));

	//// Sprint
	//CHelpers::GetAsset<UInputAction>(&IA_Sprint, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Sprint.IA_Sprint'"));

	// Crouch
	CHelpers::GetAsset<UInputAction>(&IA_Crouch, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Crouch.IA_Crouch'"));

	// Walk
	CHelpers::GetAsset<UInputAction>(&IA_Crouch, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Walk.IA_Walk'"));

	// Jump
	CHelpers::GetAsset<UInputAction>(&IA_Jump, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Jump.IA_Jump'"));

}

//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
void UCMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UCMovementComponent, Pressed);

}
