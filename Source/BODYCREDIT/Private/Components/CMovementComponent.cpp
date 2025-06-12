#include "Components/CMovementComponent.h"
#include "Global.h"
#include "InputAction.h"
#include "EnhancedInputComponent.h"
#include "Characters/CNox.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CNoxHPComponent.h"

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

	if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRecoverTimerHandle))
	{
		UCNoxHPComponent* hp = CHelpers::GetComponent<UCNoxHPComponent>(OwnerCharacter);
		CheckNull(hp);

		if (hp->Stamina >= hp->MaxStamina or bSprint)
			GetWorld()->GetTimerManager().ClearTimer(StaminaRecoverTimerHandle);
	}

	// MaxWalkSpeed 보간 처리
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = FMath::FInterpTo(OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed, DesiredMaxWalkSpeed, DeltaTime, InterpSpeed);

	//if (bIsSliding)
	//{
	//	SlideElapsedTime += DeltaTime;

	//	FVector CurrentVelocity = OwnerCharacter->GetVelocity();
	//	FVector NewVelocity = CurrentVelocity - (CurrentVelocity.GetSafeNormal() * SlideFriction * DeltaTime);

	//	if (NewVelocity.Size() <= MinSlideSpeed)
	//	{
	//		OffSlide(FInputActionValue());
	//	}
	//	else
	//	{
	//		OwnerCharacter->GetCharacterMovement()->Velocity = NewVelocity;
	//	}
	//}

	if (bIsSliding)
	{
		SlideElapsedTime += DeltaTime;

		UCharacterMovementComponent* MoveComp = OwnerCharacter->GetCharacterMovement();
		FVector CurrentVelocity = MoveComp->Velocity;
		FVector SlideDirection = CurrentVelocity.GetSafeNormal();

		// 🔥 핵심! 높이 차이로 오르막/내리막 판단
		float HeightDelta = OwnerCharacter->GetActorLocation().Z - LastSlideLocation.Z;
		LastSlideLocation = OwnerCharacter->GetActorLocation(); // 갱신

		float SlopeFactor = 1.0f;

		if (HeightDelta > 0) SlopeFactor = 10.0f; // 오르막
		else if (HeightDelta < 0) SlopeFactor = 0.5f; // 내리막
		else SlopeFactor = 2.0f; // 평지

		// 감속 적용
		FVector NewVelocity = CurrentVelocity - (SlideDirection * SlideFriction * DeltaTime * SlopeFactor);
		NewVelocity.Z = 0.0f;
		MoveComp->Velocity = NewVelocity;

		if (NewVelocity.Size2D() <= MinSlideSpeed)
			OffSlide(FInputActionValue());
	}

	if (UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter))
		if (weapon->IsBowMode()) return;

	if (UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter))
		camera->SetFieldOfView(FMath::FInterpTo(camera->FieldOfView, TargetFOV, DeltaTime, FOVInterpSpeed));

}

void UCMovementComponent::BindInput(UEnhancedInputComponent* InEnhancedInputComponent)
{
	// Movement
	InEnhancedInputComponent->BindAction(IA_Movement, ETriggerEvent::Triggered, this, &UCMovementComponent::OnMovement);
	InEnhancedInputComponent->BindAction(IA_Movement, ETriggerEvent::Completed, this, &UCMovementComponent::OffMovement);

	// Look
	InEnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UCMovementComponent::OnHorizontalLook);
	InEnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &UCMovementComponent::OnVerticalLook);

	//// Sprint // 보류
	InEnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Started, this, &UCMovementComponent::OnSprint);
	InEnhancedInputComponent->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &UCMovementComponent::OnReset);

	// Crouch
	InEnhancedInputComponent->BindAction(IA_Crouch, ETriggerEvent::Started, this, &UCMovementComponent::OnCrouch);

	// Slide
	InEnhancedInputComponent->BindAction(IA_Slide, ETriggerEvent::Started, this, &UCMovementComponent::OnSlide);
	InEnhancedInputComponent->BindAction(IA_Slide, ETriggerEvent::Completed, this, &UCMovementComponent::OffSlide);

	// Jump
	InEnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &UCMovementComponent::OnJump);

}

void UCMovementComponent::OnMovement(const FInputActionValue& InVal)
{
	CheckFalse(bCanMove);
	CheckTrue(bSlide);

	// 캐릭터의 정면 방향을 가져오기 위해 컨트롤러의 회전 값을 구해서 Z축만을 사용
	FRotator rot = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);

	const FVector2D input = InVal.Get<FVector2D>();

	if (input.X > 0)
	{
		bForward = true;

		if (bCrouch)
		{
			SetCrouchWalkForwardSpeed();

			if (bSprint)
			{
				OnCrouch(FInputActionValue());

				SetSprintSpeed();
			}
		}
		else if (bSprint)
		{
			SetSprintSpeed();
		}
		else SetStandRunForwardSpeed();

		//if (bSprint)
		//{
		//	if (bCrouch)
		//		OnCrouch(FInputActionValue());

		//	SetSprintSpeed();
		//}
		//else SetStandRunForwardSpeed();

		// Forward
		OwnerCharacter->AddMovementInput(FQuat(rot).GetForwardVector(), input.X);
	}
	else if (input.X == 0)
	{
		bForward = false;

		OnReset(FInputActionValue());

		SetStandRunRLwardSpeed();
	}
	else
	{
		bForward = false;

		if (bCrouch)
			SetCrouchWalkBackwardSpeed();
		else SetStandRunBackwardSpeed();

		// back
		OwnerCharacter->AddMovementInput(FQuat(rot).GetForwardVector(), input.X);
	}

	// Right Or Left
	OwnerCharacter->AddMovementInput(FQuat(rot).GetRightVector(), input.Y);
	
	//if (input.Y > 0)
	//{
	//	if (input.X == 0)
	//		SetStandRunRLwardSpeed();

	//	// Right
	//	OwnerCharacter->AddMovementInput(FQuat(rot).GetRightVector(), input.Y);
	//}
	//else
	//{
	//	if (input.X == 0)
	//		SetStandRunRLwardSpeed();

	//	// Left
	//	OwnerCharacter->AddMovementInput(FQuat(rot).GetRightVector(), input.Y);
	//}

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
	// 유효성 확인
	CheckNull(CHelpers::GetComponent<UCNoxHPComponent>(OwnerCharacter));

	// Delegate에 람다 바인딩
	FTimerDelegate StaminaDrainDelegate;
	StaminaDrainDelegate.BindLambda([this]()
		{
			UCNoxHPComponent* component = CHelpers::GetComponent<UCNoxHPComponent>(OwnerCharacter);

			float NewStamina = component->Stamina - StaminaDrainAmount;
			NewStamina = FMath::Max(0.0f, NewStamina);

			component->SetStamina(NewStamina);

			// 스태미나가 0이 되면 자동으로 정지
			if (NewStamina <= 0.0f)
				OnReset(FInputActionValue());
		});

	// 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(
		StaminaDrainTimerHandle,
		StaminaDrainDelegate,
		StaminaDrainInterval,
		true
	);

	if (bSlide)
		OffSlide(FInputActionValue());

	bSprint = true;

	CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter)->SubAction_Released();

}

void UCMovementComponent::OnReset(const FInputActionValue& InVal)
{
	bSprint = false;

	// 스프린트 타이머 정지
	GetWorld()->GetTimerManager().ClearTimer(StaminaDrainTimerHandle);

	// 회복 타이머 중복 방지
	if (GetWorld()->GetTimerManager().IsTimerActive(StaminaRecoverTimerHandle) or
		GetWorld()->GetTimerManager().IsTimerActive(StaminaRecoverDelayTimerHandle))
	{
		return;
	}

	// 1초 후에 회복 타이머를 시작하는 람다 등록
	GetWorld()->GetTimerManager().SetTimer(
		StaminaRecoverDelayTimerHandle,
		FTimerDelegate::CreateLambda([this]()
			{
				FTimerDelegate RecoverDelegate;
				RecoverDelegate.BindLambda([this]()
					{
						UCNoxHPComponent* component = CHelpers::GetComponent<UCNoxHPComponent>(OwnerCharacter);
						CheckNull(component);

						float NewStamina = component->Stamina + StaminaRecoverAmount;
						float MaxStamina = component->MaxStamina;

						NewStamina = FMath::Min(NewStamina, MaxStamina);
						component->SetStamina(NewStamina);
					});

				// 회복 타이머 시작
				GetWorld()->GetTimerManager().SetTimer(
					StaminaRecoverTimerHandle,
					RecoverDelegate,
					StaminaRecoverInterval,
					true
				);

			}),
		1.0f,  // 딜레이 시간
		false  // 반복 없음
	);
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

	CheckFalse(bCanMove);

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

void UCMovementComponent::OnSlide(const FInputActionValue& InVal)
{
	CheckTrue(OwnerCharacter->GetVelocity().Size2D() < 510);
	if (UCNoxHPComponent* comp = CHelpers::GetComponent<UCNoxHPComponent>(OwnerCharacter))
	{
		CheckTrue(comp->Stamina < 20);
		comp->Stamina -= 20;
		comp->SetStamina(FMath::Clamp(comp->Stamina, 0, comp->MaxStamina));

		//if (bSprint)
		//{
		//	OffSlide(FInputActionValue());

		//	return;
		//}

		//OwnerCharacter->Crouch();

		bSlide = true;

		//if (!bIsSliding && OwnerCharacter->GetCharacterMovement()->Velocity.Size() >= SlideInitialSpeed)
		//{
		//	bSlide = true;
		//	SlideElapsedTime = 0.0f;

		//	OwnerCharacter->Crouch();

		//	// 슬라이딩 중 마찰력 제거
		//	OwnerCharacter->GetCharacterMovement()->BrakingFrictionFactor = 0.0f;

		//	// 슬라이딩 방향은 현재 속도 방향
		//	FRotator rot = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);

		//	OwnerCharacter->GetCharacterMovement()->AddImpulse(FQuat(rot).GetForwardVector() * OwnerCharacter->GetVelocity().Size2D(), true);
		//}

		// 슬라이딩 조건: 지면 + 달리기 중 + 이미 슬라이딩 중이 아님
		if (bIsSliding or !OwnerCharacter->GetCharacterMovement()->IsMovingOnGround() or !bSprint)
			return;

		LastSlideLocation = OwnerCharacter->GetActorLocation();

		bIsSliding = true;
		SlideElapsedTime = 0.0f;

		// 마찰력 제거로 감속 늦춤
		OwnerCharacter->GetCharacterMovement()->BrakingFrictionFactor = 0.0f;
		OwnerCharacter->GetCharacterMovement()->GroundFriction = 0.0f;
		OwnerCharacter->GetCharacterMovement()->BrakingDecelerationWalking = 0.0f;

		// MovementMode 유지 (공중 상태 방지)
		OwnerCharacter->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

		// 지면 방향으로 밀기
		FVector SlideDirection = OwnerCharacter->GetVelocity().GetSafeNormal();
		OwnerCharacter->GetCharacterMovement()->AddImpulse(SlideDirection * SlideInitialSpeed, true);
	}

}

void UCMovementComponent::OffSlide(const FInputActionValue& InVal)
{
	//OwnerCharacter->UnCrouch();

	bSlide = false;

	//bIsSliding = false;
	//
	//OwnerCharacter->UnCrouch();

	//// 마찰력 원상복귀
	//OwnerCharacter->GetCharacterMovement()->BrakingFrictionFactor = 2.0f;

	//// 슬라이드 종료 후 기본 이동 속도 복원
	//OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = 500.0f;

	bIsSliding = false;

	// 마찰력 복구
	OwnerCharacter->GetCharacterMovement()->BrakingFrictionFactor = 2.0f;
	OwnerCharacter->GetCharacterMovement()->GroundFriction = 8.0f;
	OwnerCharacter->GetCharacterMovement()->BrakingDecelerationWalking = 2048.0f;

	// 걷기 속도 복구
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)ESpeedType::STAND_RUN_FWD];

}

void UCMovementComponent::OnJump(const FInputActionValue& InVal)
{
	if (bSlide)
		OffSlide(FInputActionValue());

	OwnerCharacter->Jump();

}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	//OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(uint8)InType];

}

void UCMovementComponent::SetStandWalkSpeed()
{
	SetSpeed(ESpeedType::STAND_WALK);

	DesiredMaxWalkSpeed = Speed[(int32)ESpeedType::STAND_WALK];

}

void UCMovementComponent::SetStandRunForwardSpeed()
{
	SetSpeed(ESpeedType::STAND_RUN_FWD);

	DesiredMaxWalkSpeed = Speed[(int32)ESpeedType::STAND_RUN_FWD];

}

void UCMovementComponent::SetStandRunBackwardSpeed()
{
	SetSpeed(ESpeedType::STAND_RUN_BWD);

	DesiredMaxWalkSpeed = Speed[(int32)ESpeedType::STAND_RUN_BWD];

}

void UCMovementComponent::SetStandRunRLwardSpeed()
{
	SetSpeed(ESpeedType::STAND_RUN_RLWD);

	DesiredMaxWalkSpeed = Speed[(int32)ESpeedType::STAND_RUN_RLWD];

}

void UCMovementComponent::SetCrouchWalkForwardSpeed()
{
	SetSpeed(ESpeedType::CROUCH_WALK_FWD);

	DesiredMaxWalkSpeed = Speed[(int32)ESpeedType::CROUCH_WALK_FWD];
	
}

void UCMovementComponent::SetCrouchWalkBackwardSpeed()
{
	SetSpeed(ESpeedType::CROUCH_WALK_BWD);

	DesiredMaxWalkSpeed = Speed[(int32)ESpeedType::CROUCH_WALK_BWD];

}

void UCMovementComponent::SetCrouchWalkRLwardSpeed()
{
	SetSpeed(ESpeedType::CROUCH_WALK_RLWD);

	DesiredMaxWalkSpeed = Speed[(int32)ESpeedType::CROUCH_WALK_RLWD];

}

void UCMovementComponent::SetSprintSpeed()
{
	SetSpeed(ESpeedType::SPRINT);

	DesiredMaxWalkSpeed = Speed[(int32)ESpeedType::SPRINT];

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

void UCMovementComponent::Dead()
{
	IA_Movement = nullptr;
	IA_Look = nullptr;

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

	// Slide
	CHelpers::GetAsset<UInputAction>(&IA_Slide , TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Slide.IA_Slide'"));

	// Jump
	CHelpers::GetAsset<UInputAction>(&IA_Jump, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Jump.IA_Jump'"));

}
