#include "Characters/CNox_Runner.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
//#include "CharacterTrajectoryComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CZoomComponent.h"
#include "Components/CMontageComponent.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Components/CNoxHPComponent.h"
#include "Widgets/Runners/CUserWidget_RunnerUI.h"
#include "Games/CMainGM.h"
#include "Inventory/AC_EquipComponent.h"
#include "Session/NetGameInstance.h"
#include "GameState_BodyCredit.h"
#include "Inventory/AC_MarketComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Characters/CNox_Controller.h"
#include "Components/CNoxObserverComp.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Lobby/LobbyWidget_Failed.h"

ACNox_Runner::ACNox_Runner()
{
	PrimaryActorTick.bCanEverTick = true;

	Init();
	CHelpers::CreateActorComponent<UCNoxHPComponent>(this, &HPComp, "HPComp");
	CHelpers::CreateActorComponent<UCNoxObserverComp>(this, &ObserverComp, "ObserverComp");

	InventoryComp = CreateDefaultSubobject<UAC_InventoryComponent>(TEXT("InventoryComp"));
	EquipComp = CreateDefaultSubobject<UAC_EquipComponent>(TEXT("EquipComp"));
	MarketComp = CreateDefaultSubobject<UAC_MarketComponent>(TEXT("MarketComp"));

	CaptureRoot = CreateDefaultSubobject<USceneComponent>(TEXT("CaptureRoot"));
	CaptureRoot->SetupAttachment(RootComponent);

	SceneCapture2D = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
	SceneCapture2D->SetupAttachment(CaptureRoot);
	
}

void ACNox_Runner::BeginPlay()
{
	Super::BeginPlay();

	CacheDefaultSkeletalMeshes();

	SceneCapture2D->ShowOnlyActorComponents(this);

	Movement->EnableControlRotation();
	// Movement->Stop();

	UNetGameInstance* GI = Cast<UNetGameInstance>(GetGameInstance());
	if (GI && EquipComp)
	{
		GI->PlayerGold = PlayerGold;
		AGameState_BodyCredit* MyGameState = GetWorld()->GetGameState<AGameState_BodyCredit>();
		for (auto& Pair : GI->SavedEquippedItems)
		{
			EquipComp->EquippedItems.Add(Pair.Key, CreateItemFromData(Pair.Value));

			MyGameState->SpawnItemHiddenFromActor(EquipComp->EquippedItems[Pair.Key], this, true);
		}


		/*for (const FItemSaveData& Data : GI->SavedInventoryItems)
		{
			UItemObject* Item = CreateItemFromData(Data);

			if (Item->IsRotated() != Data.bRotated)
				Item->Rotate();

			FInventoryTile StartTile(Data.StartPosition.X, Data.StartPosition.Y);
			int32 Index = InventoryComp->TileToIndex(StartTile);

			if (InventoryComp->IsRoomAvailable(Item, Index))
			{
				InventoryComp->AddItemAt(Item, Index);
				MyGameState->SpawnItemHiddenFromActor(Item, this, true);
			}
			else
			{
				InventoryComp->TryAddItem(Item);
			}
		}*/
		
	}

	State->OnStateTypeChanged.AddDynamic(this, &ACNox_Runner::OnStateTypeChanged);

	// 서버 관련 메시 숨김 처리 함수
	//GetMesh()->SetOnlyOwnerSee();
	//GetMesh()->SetOwnerNoSee();

	//TPSCamera->SetActive(false);
	//FPSCamera->bUsePawnControlRotation = true;
	//// 머리 본 숨김
	//GetMesh()->HideBoneByName(FName("neck_01"), EPhysBodyOp::PBO_None);
}

float ACNox_Runner::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                               class AController* EventInstigator, AActor* DamageCauser)
{
	HPComp->TakeDamage(DamageAmount);
	State->SetHittedMode();
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACNox_Runner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldRotateToTarget)
	{
		AController* controller = GetController<AController>();
		FRotator Current = controller->GetControlRotation();
		FRotator NewRot = FMath::RInterpTo(Current, TargetControlRotation, DeltaTime, RotationInterpSpeed);

		controller->SetControlRotation(NewRot);

		if (NewRot.Equals(TargetControlRotation, 0.5f))
		{
			bShouldRotateToTarget = false;
		}
	}
	
	CheckFootstep(LeftFootSocketName, bLeftFootOnGround);
	CheckFootstep(RightFootSocketName, bRightFootOnGround);

//#if WITH_EDITOR
//	const FVector Location = GetActorLocation();
//
//	// 1. Actor Rotation (청록색)
//	const FRotator ActorRot = GetActorRotation();
//	const FVector ActorDir = ActorRot.Vector() * 300.0f;
//	DrawDebugDirectionalArrow(GetWorld(), Location, Location + ActorDir, 100.f, FColor::Cyan, false, -1.f, 0, 2.f);
//	DrawDebugString(GetWorld(), Location + ActorDir + FVector(0, 0, 20.f),
//	                FString::Printf(TEXT("ActorYaw: %.1f"), ActorRot.Yaw), nullptr, FColor::Cyan, 0.f, true);
//
//	// 2. Control Rotation (노란색)
//	const FRotator ControlRot = GetControlRotation();
//	const FVector ControlDir = ControlRot.Vector() * 300.0f;
//	DrawDebugDirectionalArrow(GetWorld(), Location, Location + ControlDir, 100.f, FColor::Yellow, false, -1.f, 0, 2.f);
//	DrawDebugString(GetWorld(), Location + ControlDir + FVector(0, 0, 40.f),
//	                FString::Printf(TEXT("ControlYaw: %.1f"), ControlRot.Yaw), nullptr, FColor::Yellow, 0.f, true);
//
//	// 3. Velocity Rotation (빨간색)
//	if (!GetVelocity().IsNearlyZero())
//	{
//		const FRotator VelocityRot = GetVelocity().ToOrientationRotator();
//		const FVector VelocityDir = VelocityRot.Vector() * 300.0f;
//		DrawDebugDirectionalArrow(GetWorld(), Location, Location + VelocityDir, 100.f, FColor::Red, false, -1.f, 0,
//		                          2.f);
//		DrawDebugString(GetWorld(), Location + VelocityDir + FVector(0, 0, 60.f),
//		                FString::Printf(TEXT("VelocityYaw: %.1f"), VelocityRot.Yaw), nullptr, FColor::Red, 0.f, true);
//	}
//#endif
}

void ACNox_Runner::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// MappingContext
	if (ACNox_Controller* PlayerController = Cast<ACNox_Controller>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(IMC_Movement, 10);
			Subsystem->AddMappingContext(IMC_Weapon, 11);
			Subsystem->AddMappingContext(IMC_Invectory, 11);
		}
	}
}

void ACNox_Runner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Movement
		Movement->BindInput(input);

		// Weapon
		Weapon->BindInput(input);

		input->BindAction(IA_Jump, ETriggerEvent::Started, this, &ACNox_Runner::OnJumpOrDodgeInput);
	}
}

void ACNox_Runner::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	// FVector2D InputDir = GetLastMovementInputVector2D();
	// FRotator ControlRot = Controller ? Controller->GetControlRotation() : GetActorRotation();

	// 예시: 캐릭터나 MovementComponent에서
	FVector2D InputDir2D = Movement->GetCachedInputDir2D();
	FRotator ControlRot = GetControlRotation();
	
	switch (InNewType)
	{
	case EStateType::Hitted:
		{
			Montage->PlayHittedMode(InputDir2D, ControlRot);
			// 1. 플레이어 컨트롤러 가져오기
			APlayerController* PlayerController = Cast<APlayerController>(GetController());
			if (PlayerController == nullptr)
			{
				return;
			}
			
			// 2. CameraShake 클래스 지정 (BP로 만든 CameraShake 사용 가능)
			TSubclassOf<UCameraShakeBase> ShakeClass = LoadClass<UCameraShakeBase>(nullptr, TEXT("/Script/Engine.Blueprint'/Game/Characters/Runner/Animations/CameraShakes/BP_NormalAttackShake.BP_NormalAttackShake_C'"));
			if (ShakeClass == nullptr)
			{
				return;
			}
			
			// 3. 카메라 쉐이크 실행
			PlayerController->ClientStartCameraShake(ShakeClass);
			break;
		}
	case EStateType::Avoid:
		Montage->PlayAvoidMode(Weapon->GetWeaponType(), InputDir2D, ControlRot);
		break;
	case EStateType::Dead:
		Montage->PlayDeadMode(InputDir2D, ControlRot);
		break;
	}

}

void ACNox_Runner::Init()
{
	UCapsuleComponent* capsule = Cast<UCapsuleComponent>(RootComponent);
	capsule->SetCollisionProfileName(FName("Player"));

	{ // Modular Character Mesh
		// Head
		GetMesh()->SetRelativeLocation(FVector(0, 0, ~84));
		GetMesh()->SetRelativeRotation(FRotator(0, ~89, 0));

		// Hair
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Hair, "Hair", GetMesh());

		// UpperBody
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &UpperBody, "UpperBody", GetMesh());

		// ChestRig
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &ChestRig, "ChestRig", GetMesh());
		
		// Arms
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Arms, "Arms", GetMesh());

		// Backpack
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Backpack, "Backpack", GetMesh());

		// LowerBody
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &LowerBody, "LowerBody", GetMesh());

		// Foot
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Foot, "Foot", GetMesh());

		// Weapon1
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Weapon1, "Weapon1", GetMesh());
		Weapon1->SetHiddenInGame(true);

		// Weapon2
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Weapon2, "Weapon2", GetMesh());
		Weapon2->SetHiddenInGame(true);

	}

	// SpringArm
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->TargetArmLength = 200;
	SpringArm->bUsePawnControlRotation = true;

	// TPSCamera
	CHelpers::CreateComponent<UCameraComponent>(this, &TPSCamera, "TPSCamera", SpringArm);
	TPSCamera->SetRelativeLocation(FVector(~79, 60, 100));
	TPSCamera->bUsePawnControlRotation = false;

	// MappingContext
	CHelpers::GetAsset<UInputMappingContext>(&IMC_Movement,
	                                         TEXT(
		                                         "/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_Movement.IMC_Movement'"));

	CHelpers::GetAsset<UInputMappingContext>(&IMC_Weapon,
										 TEXT(
											 "/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_Weapon.IMC_Weapon'"));

	CHelpers::GetAsset<UInputMappingContext>(&IMC_Invectory,
										 TEXT(
											 "/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_Runner.IMC_Runner'"));

	// Jump
	CHelpers::GetAsset<UInputAction>(&IA_Jump, TEXT("/Script/EnhancedInput.InputAction'/Game/Inputs/IA_Jump.IA_Jump'"));

	
	// // State
	// CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");

	// Movement
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");

	// Weapon
	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");

	// Zoom
	CHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");

	// Montage
	CHelpers::CreateActorComponent<UCMontageComponent>(this, &Montage, "Montage");

}

FVector2D ACNox_Runner::GetLastMovementInputVector2D() const
{
	FVector Dir3D = GetLastMovementInputVector();
	return FVector2D(Dir3D.X, Dir3D.Y);
}

void ACNox_Runner::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Movement->Dead();
	State->SetDeadMode();
}

void ACNox_Runner::End_Avoid()
{
	State->SetIdleMode();
}

void ACNox_Runner::End_Hitted()
{
	State->SetIdleMode();
}

void ACNox_Runner::End_Dead()
{
	if (FailedWidget)
	{
		FailedWidget->AddToViewport();
		FailedWidget->Refresh();
		return;
	}
	
	if (FailedWidgetClass)
	{
		FailedWidget = CreateWidget<ULobbyWidget_Failed>(GetWorld(), FailedWidgetClass);
		FailedWidget->AddToViewport();
	}
	
}

void ACNox_Runner::ShowPlayerMainUI()
{
	// 위젯 클래스가 유효한지 확인
	if (RunnerUIClass)
	{
		// 위젯 생성
		RunnerUIWidget = CreateWidget<UCUserWidget_RunnerUI>(GetWorld(), RunnerUIClass);

		if (RunnerUIWidget)
		{
			// 화면에 추가
			RunnerUIWidget->AddToViewport();

			// 캐릭터의 HealthComponent를 바인딩
			RunnerUIWidget->BindToHealthComponent(HPComp);
		}
	}
}

void ACNox_Runner::RemovePlayerMainUI()
{
	if (RunnerUIWidget)
	{
		RunnerUIWidget->RemoveFromParent();
		RunnerUIWidget = nullptr;
	}
}

void ACNox_Runner::MakeMemoryPiece(const EMemoryTriggerType& Trigger)
{
	MainGM->RegisterMemoryFromPlayer(this, Trigger);
}

UItemObject* ACNox_Runner::CreateItemFromData(const FItemSaveData& Data)
{
	UItemObject* NewItem = NewObject<UItemObject>();
	NewItem->ImportData(Data);
	return NewItem;
}

void ACNox_Runner::RegisterAttack()
{
	ObserverComp->RegisterAttack();
}

void ACNox_Runner::RegisterLooting()
{
	ObserverComp->RegisterLooting();
}

void ACNox_Runner::OnJumpOrDodgeInput()
{
	CheckNull(Weapon);
	CheckNull(State);
	CheckNull(Movement);

	EWeaponType WeaponType = Weapon->GetWeaponType();

	// Bow: SubAction(줌/시위) 모드에서만 회피, 그 외엔 점프
	if (WeaponType == EWeaponType::BOW && Weapon->IsBowSubActionActive())
	{
		State->SetAvoidMode();
		return;
	}

	// Katana: 더블탭이면 회피, 아니면 (딜레이 후) 점프
	if (WeaponType == EWeaponType::KATANA)
	{
		float Now = GetWorld()->GetTimeSeconds();

		// 이미 대기 중인 첫 입력(딜레이 타이머) 있다면 = 더블탭 성공
		if (bPendingJump && (Now - LastJumpInputTime) < DoubleTapThreshold)
		{
			bPendingJump = false;
			GetWorld()->GetTimerManager().ClearTimer(JumpDelayHandle);
			State->SetAvoidMode(); // 회피 실행
			return;
		}

		// 첫 입력(딜레이 타이머 시작)
		LastJumpInputTime = Now;
		bPendingJump = true;
		GetWorld()->GetTimerManager().SetTimer(
			JumpDelayHandle, this, &ACNox_Runner::DoJumpIfNoDoubleTap, DoubleTapThreshold, false
		);
		return;
	}

	// 그 외 무기/비무장: 즉시 점프
	Movement->OnJump(FInputActionValue());
}

// 딜레이 끝날 때 점프 실행(더블탭 없었을 때)
void ACNox_Runner::DoJumpIfNoDoubleTap()
{
	if (bPendingJump)
	{
		bPendingJump = false;
		Movement->OnJump(FInputActionValue());
	}
}

bool ACNox_Runner::IsDoubleTap()
{
	float Now = GetWorld()->GetTimeSeconds();
	if (LastJumpInputTime > 0 && (Now - LastJumpInputTime) < DoubleTapThreshold)
	{
		LastJumpInputTime = -1.0f; // 리셋
		return true;
	}
	LastJumpInputTime = Now;
	return false;
}

void ACNox_Runner::CacheDefaultSkeletalMeshes()
{
	DefaultMeshes.Add(EPlayerPart::Head, Hair->GetSkeletalMeshAsset());
	DefaultMeshes.Add(EPlayerPart::Body, UpperBody->GetSkeletalMeshAsset());
	DefaultMeshes.Add(EPlayerPart::ChestRigs, ChestRig->GetSkeletalMeshAsset());
	DefaultMeshes.Add(EPlayerPart::Arm, Arms->GetSkeletalMeshAsset());
	DefaultMeshes.Add(EPlayerPart::Leg, LowerBody->GetSkeletalMeshAsset());
	DefaultMeshes.Add(EPlayerPart::Backpack, Backpack->GetSkeletalMeshAsset());
	DefaultMeshes.Add(EPlayerPart::Weapon1, Weapon1->GetSkeletalMeshAsset());
	DefaultMeshes.Add(EPlayerPart::Weapon2, Weapon2->GetSkeletalMeshAsset());
}

void ACNox_Runner::CheckFootstep(FName FootSocketName, bool& bWasOnGround)
{
	FVector FootLocation = GetMesh()->GetSocketLocation(FootSocketName);
	FVector Start        = FootLocation + FVector(0, 0, 5);
	FVector End          = FootLocation - FVector(0, 0, 20);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit          = GetWorld()->LineTraceSingleByChannel(
							 Hit, Start, End, ECC_Visibility, Params);
	bool bNowOnGround = bHit && Hit.bBlockingHit;

	if (!bWasOnGround && bNowOnGround)
	{
		// 발소리 재생
		PlayFootstepSound(FootLocation);

		// 상태별 노이즈 세기 적용
		float Loudness = GetFootstepNoiseLoudness();
		MakeNoise(Loudness, this, FootLocation);
	}

	bWasOnGround = bNowOnGround;
	
	// FVector FootLocation = GetMesh()->GetSocketLocation(FootSocketName);
	// FVector Start = FootLocation + FVector(0, 0, 5); // 약간 위에서 시작
	// FVector End = FootLocation - FVector(0, 0, 20); // 아래로 쏨
	//
	// FHitResult Hit;
	// FCollisionQueryParams Params;
	// Params.AddIgnoredActor(this);
	//
	// bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	//
	// bool bNowOnGround = bHit && Hit.bBlockingHit;
	//
	// // 떨어져 있다가 이번 프레임에 바닥에 닿았으면 (= 발 디딤)
	// if (!bWasOnGround && bNowOnGround)
	// {
	// 	// 여기서 발소리 재생!
	// 	PlayFootstepSound(FootLocation);
	//
	// 	// MakeNoise도 여기서 호출 가능
	// 	MakeNoise(1.0f, this, FootLocation);
	// }
	//
	// bWasOnGround = bNowOnGround;
}

void ACNox_Runner::PlayFootstepSound(const FVector& Location)
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	bool bCrouched                     = MoveComp->IsCrouching();
	float Speed                          = MoveComp->Velocity.Size();
	bool bIsSprinting                    = (Speed > SprintSpeedThreshold);

	TArray<USoundBase*>* SoundArray = nullptr;

	if (bCrouched)
	{
		SoundArray = &CrouchFootstepSounds;
	}
	else if (bIsSprinting)
	{
		SoundArray = &SprintFootstepSounds;
	}
	else
	{
		SoundArray = &WalkFootstepSounds;
	}

	if (SoundArray && SoundArray->Num() > 0)
	{
		int32 Index = FMath::RandRange(0, SoundArray->Num() - 1);
		USoundBase* SelectedSound = (*SoundArray)[Index];
		if (SelectedSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(), SelectedSound, Location);
		}
	}
	
	// if (FootstepSounds.Num() > 0)
	// {
	// 	int32 Index = FMath::RandRange(0, FootstepSounds.Num() - 1);
	// 	USoundBase* SelectedSound = FootstepSounds[Index];
	// 	if (SelectedSound)
	// 	{
	// 		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SelectedSound, Location);
	// 	}
	// }
}

float ACNox_Runner::GetFootstepNoiseLoudness() const
{
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	bool bCrouched  = MoveComp->IsCrouching();
	float Speed = MoveComp->Velocity.Size();
	bool bIsSprinting = (Speed > SprintSpeedThreshold);

	if (bCrouched)
	{
		return CrouchNoiseLoudness;
	}
	else if (bIsSprinting)
	{
		return SprintNoiseLoudness;
	}
	return WalkNoiseLoudness;
}

void ACNox_Runner::Reset()
{
	HPComp->InitStatus();
	State->SetIdleMode();
	Movement->Revive();
	Weapon->SetUnarmedMode();
}
