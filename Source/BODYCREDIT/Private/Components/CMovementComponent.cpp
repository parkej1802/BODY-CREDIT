#include "Components/CMovementComponent.h"
#include "Global.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Characters/CNox.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

UCMovementComponent::UCMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	Init();

}

void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	EnableControlRotation();

}

void UCMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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

	// Sprint
	InEnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Started, this, &UCMovementComponent::OnSprint);
	InEnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &UCMovementComponent::OnReset);

	// Crouch
	InEnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &UCMovementComponent::OnCrouch);

	// Jump
	InEnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &UCMovementComponent::OnJump);

}

void UCMovementComponent::OnMoveForward(const FInputActionValue& InVal)
{
	CheckFalse(bCanMove);

	const FVector2D input = InVal.Get<FVector2D>();
	CheckTrue(input.X == 0);

	bMove = true;
	TargetFOV = RunFOV;

	// 이동 방향 계산 (카메라 전방 기준)
	const float yaw = OwnerCharacter->GetControlRotation().Yaw;
	const FVector direction = FQuat(FRotator(0, yaw, 0)).GetForwardVector();

	// 이동 입력 적용
	OwnerCharacter->AddMovementInput(direction, input.X);

}

void UCMovementComponent::OnMoveRight(const FInputActionValue& InVal)
{
	CheckFalse(bCanMove);

	const FVector2D input = InVal.Get<FVector2D>();
	CheckTrue(input.Y == 0);

	bMove = true;

	// 이동 방향 계산 (카메라 전방 기준)
	const float yaw = OwnerCharacter->GetControlRotation().Yaw;
	const FVector direction = FQuat(FRotator(0, yaw, 0)).GetRightVector();

	// 이동 입력 적용
	OwnerCharacter->AddMovementInput(direction, input.Y);

}

void UCMovementComponent::OffMovement(const FInputActionValue& InVal)
{
	bMove = false;
	TargetFOV = DefaultFOV;

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
	CheckFalse(bMove);

	if (bCrouch)
		OnCrouch(FInputActionValue());

	SetSprintState(true);

}

void UCMovementComponent::OnReset(const FInputActionValue& InVal)
{
	if (bCrouch) // Crouch 상태 리셋
		SetCrouchSpeed();
	else SetRunSpeed();

	bMove = false;
	bSprint = false;
	TargetFOV = DefaultFOV;

}

void UCMovementComponent::OnCrouch(const FInputActionValue& InVal)
{
	if (bSprint) // Sprint 상태 리셋
		OnReset(FInputActionValue());

	if (OwnerCharacter->GetCharacterMovement()->IsFalling())
	{
		const FVector launch = OwnerCharacter->GetActorForwardVector() * 500.f;
		OwnerCharacter->LaunchCharacter(launch, false, false);

		return;
	}

	SetCrouchState(!bCrouch);

}

void UCMovementComponent::OnJump(const FInputActionValue& InVal)
{
	OwnerCharacter->Jump();

}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(uint8)InType];

}

void UCMovementComponent::SetCrouchSpeed()
{
	SetSpeed(ESpeedType::CROUCH);

}

void UCMovementComponent::SetRunSpeed()
{
	SetSpeed(ESpeedType::RUN);

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

void UCMovementComponent::Init()
{
	// Movement
	CHelpers::GetAsset<UInputAction>(&IA_Movement, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Movement.IA_Movement'"));

	// Look
	CHelpers::GetAsset<UInputAction>(&IA_Look, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Look.IA_Look'"));

	// Sprint
	CHelpers::GetAsset<UInputAction>(&IA_Sprint, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Sprint.IA_Sprint'"));

	// Crouch
	CHelpers::GetAsset<UInputAction>(&IA_Crouch, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Crouch.IA_Crouch'"));

	// Jump
	CHelpers::GetAsset<UInputAction>(&IA_Jump, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Jump.IA_Jump'"));

}

void UCMovementComponent::SetSprintState(bool bEnable)
{
	bSprint = bEnable;
	SetSprintSpeed();
	TargetFOV = bEnable ? SprintFOV : RunFOV;

}

void UCMovementComponent::SetCrouchState(bool bEnable)
{
	bCrouch = bEnable;

	if (bCrouch)
	{
		OwnerCharacter->Crouch();
		SetCrouchSpeed();
	}
	else
	{
		OwnerCharacter->UnCrouch();
		SetRunSpeed();
	}

}
