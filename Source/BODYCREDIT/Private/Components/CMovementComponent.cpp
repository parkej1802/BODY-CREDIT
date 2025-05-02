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

	DisableControlRotation();

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

	// Look
	InEnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UCMovementComponent::OnHorizontalLook);
	InEnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UCMovementComponent::OnVerticalLook);

	// Sprint
	InEnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Triggered, this, &UCMovementComponent::OnSprint);
	InEnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &UCMovementComponent::OnReset);

	// Crouch
	InEnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &UCMovementComponent::OnCrouch);

	// Jump
	InEnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &UCMovementComponent::OnJump);

}

void UCMovementComponent::OnMoveForward(const FInputActionValue& InVal)
{
	CheckFalse(bCanMove);
	CheckTrue(InVal.Get<FVector2D>().X == 0);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	OwnerCharacter->AddMovementInput(direction, InVal.Get<FVector2D>().X);

}

void UCMovementComponent::OnMoveRight(const FInputActionValue& InVal)
{
	CheckFalse(bCanMove);
	CheckTrue(InVal.Get<FVector2D>().Y == 0);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(direction, InVal.Get<FVector2D>().Y);

}

void UCMovementComponent::OnHorizontalLook(const FInputActionValue& InVal)
{
	CheckTrue(bFixedCamera);
	CheckTrue(InVal.Get<FVector2D>().X == 0);

	OwnerCharacter->AddControllerYawInput(InVal.Get<FVector2D>().X * HorizontalLook * GetWorld()->GetDeltaSeconds());

}

void UCMovementComponent::OnVerticalLook(const FInputActionValue& InVal)
{
	CheckTrue(bFixedCamera);
	CheckTrue(InVal.Get<FVector2D>().Y == 0);

	OwnerCharacter->AddControllerPitchInput(InVal.Get<FVector2D>().Y * VerticalLook * GetWorld()->GetDeltaSeconds());

}

void UCMovementComponent::OnSprint(const FInputActionValue& InVal)
{
	SetSprintSpeed();

	TargetFOV = SprintFOV;

}

void UCMovementComponent::OnReset(const FInputActionValue& InVal)
{
	SetRunSpeed();

	TargetFOV = DefaultFOV;

}

void UCMovementComponent::OnCrouch(const FInputActionValue& InVal)
{
	if (OwnerCharacter->GetCharacterMovement()->IsFalling())
	{
		OwnerCharacter->LaunchCharacter(OwnerCharacter->GetActorForwardVector() * 1000, false, false);

		return;
	}

	bCrouch = !bCrouch;

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
