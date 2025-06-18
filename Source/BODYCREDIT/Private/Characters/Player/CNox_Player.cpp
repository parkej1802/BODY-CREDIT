#include "Characters/Player/CNox_Player.h"
#include "Global.h"
#include "Characters/Player/CAnimInstance.h"
#include "Characters/Player/CAnimInstance_Arms.h"
#include "Components/CGunComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

ACNox_Player::ACNox_Player()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetCapsuleComponent());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &FPSArms, "FPSArms", Camera);

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

	CHelpers::CreateActorComponent<UCGunComponent>(this, &Weapon, "Weapon");
	
	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/Characters/Player/ABP_CAnimInstance.ABP_CAnimInstance_C'");
	GetMesh()->SetAnimClass(animInstance);
	
	GetCharacterMovement()->MaxWalkSpeed = 500;

	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->TargetArmLength = 200;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	
	Arms->SetRelativeLocation(FVector(-14.25f, -5.85f, -156.935f));
	Arms->SetRelativeRotation(FRotator(-0.5f, -11.85f, -1.2f));
	Arms->SetVisibility(false);

	TSubclassOf<UCAnimInstance_Arms> armsAnimInstance;
	CHelpers::GetClass<UCAnimInstance_Arms>(&armsAnimInstance, "/Script/Engine.AnimBlueprint'/Game/Characters/Player/ABP_CAnimInstance_Arms.ABP_CAnimInstance_Arms_C'");
	Arms->SetAnimClass(armsAnimInstance);
}

void ACNox_Player::BeginPlay()
{	
	Super::BeginPlay();

	GetController<APlayerController>()->PlayerCameraManager->ViewPitchMin = PitchRange.X;
	GetController<APlayerController>()->PlayerCameraManager->ViewPitchMax = PitchRange.Y;
}

void ACNox_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACNox_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACNox_Player::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACNox_Player::OnMoveRight);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACNox_Player::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACNox_Player::OnVerticalLook);

	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Pressed, this, &ACNox_Player::OnRun);
	PlayerInputComponent->BindAction("Run", EInputEvent::IE_Released, this, &ACNox_Player::OffRun);

	// PlayerInputComponent->BindAction("AR4", EInputEvent::IE_Pressed, Weapon, &UCGunComponent::SetARMode);
	// PlayerInputComponent->BindAction("AK47", EInputEvent::IE_Pressed, Weapon, &UCGunComponent::SetAK47Mode);
	// PlayerInputComponent->BindAction("Pistol", EInputEvent::IE_Pressed, Weapon, &UCGunComponent::SetPistolMode);
	//
	// PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, Weapon, &UCGunComponent::Begin_Aim);
	// PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, Weapon, &UCGunComponent::End_Aim);
	//
	// PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, Weapon, &UCGunComponent::Begin_Fire);
	// PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, Weapon, &UCGunComponent::End_Fire);
	//
	// PlayerInputComponent->BindAction("AutoFire", EInputEvent::IE_Pressed, Weapon, &UCGunComponent::ToggleAutoFire);
	//
	// PlayerInputComponent->BindAction("Reload", EInputEvent::IE_Pressed, Weapon, &UCGunComponent::Reload);
}

void ACNox_Player::OnMoveForward(float InAxisValue)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	AddMovementInput(direction, InAxisValue);
}

void ACNox_Player::OnMoveRight(float InAxisValue)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, InAxisValue);
}

void ACNox_Player::OnHorizontalLook(float InAxisValue)
{
	AddControllerYawInput(InAxisValue);
}

void ACNox_Player::OnVerticalLook(float InAxisValue)
{
	AddControllerPitchInput(InAxisValue);
}

void ACNox_Player::OnRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 600;
}

void ACNox_Player::OffRun()
{
	GetCharacterMovement()->MaxWalkSpeed = 400;
}