#include "Components/CMovementComponent.h"
#include "Global.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Characters/CNox.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CWeaponComponent.h"
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

	if (UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter))
		if (weapon->IsBowMode()) return;

	if (UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter))
		camera->SetFieldOfView(FMath::FInterpTo(camera->FieldOfView, TargetFOV, DeltaTime, FOVInterpSpeed));

}

void UCMovementComponent::BindInput(UEnhancedInputComponent* InEnhancedInputComponent)
{
	// Movement
	InEnhancedInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &UCMovementComponent::OnMovement);
	//InEnhancedInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &UCMovementComponent::OnMoveRight);
	InEnhancedInputComponent->BindAction(IA_Movement, ETriggerEvent::Completed, this, &UCMovementComponent::OffMovement);

	// Look
	InEnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UCMovementComponent::OnHorizontalLook);
	InEnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UCMovementComponent::OnVerticalLook);

	//// Sprint // 보류
	InEnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Started, this, &UCMovementComponent::OnSprint);
	InEnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &UCMovementComponent::OnReset);

	// Crouch
	InEnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &UCMovementComponent::OnCrouch);

	// Jump
	InEnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &UCMovementComponent::OnJump);

}

void UCMovementComponent::OnMovement(const FInputActionValue& InVal)
{
	CheckFalse(bCanMove);

	// 캐릭터의 정면 방향을 가져오기 위해 컨트롤러의 회전 값을 구해서 Z축만을 사용
	FRotator rot = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);

	const FVector2D input = InVal.Get<FVector2D>();

	if (input.X >= 0)
	{
		bSprint ? SetSprintSpeed() : SetMoveForwardSpeed();

		// Forward
		OwnerCharacter->AddMovementInput(FQuat(rot).GetForwardVector(), input.X);
	}
	else
	{
		SetMoveBackwardSpeed();

		// back
		OwnerCharacter->AddMovementInput(FQuat(rot).GetForwardVector(), input.X);
	}
	
	if (input.Y > 0)
	{
		if (input.X == 0)
			SetMoveRLSpeed();

		// Right
		OwnerCharacter->AddMovementInput(FQuat(rot).GetRightVector(), input.Y);
	}
	else
	{
		if (input.X == 0)
			SetMoveRLSpeed();

		// Left
		OwnerCharacter->AddMovementInput(FQuat(rot).GetRightVector(), input.Y);
	}

}

void UCMovementComponent::OffMovement(const FInputActionValue& InVal)
{
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
	bSprint = true;

}

void UCMovementComponent::OnReset(const FInputActionValue& InVal)
{
	bSprint = false;

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

	if (bCrouch)
	{
		bCrouch = false;
		OwnerCharacter->UnCrouch();
	}
	else
	{
		bCrouch = true;
		OwnerCharacter->Crouch();
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
	CHelpers::GetAsset<UInputAction>(&IA_Sprint, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Sprint.IA_Sprint'"));

	// Crouch
	CHelpers::GetAsset<UInputAction>(&IA_Crouch, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Crouch.IA_Crouch'"));

	// Walk
	CHelpers::GetAsset<UInputAction>(&IA_Walk , TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Walk.IA_Walk'"));

	// Jump
	CHelpers::GetAsset<UInputAction>(&IA_Jump, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Jump.IA_Jump'"));

}
