#include "Characters/CNox_Runner.h"
#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/CMovementComponent.h"

ACNox_Runner::ACNox_Runner()
{
	PrimaryActorTick.bCanEverTick = true;

	Init();

}

void ACNox_Runner::BeginPlay()
{
	Super::BeginPlay();

}

void ACNox_Runner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACNox_Runner::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// MappingContext
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(MappingContext, 0);
	}

}

void ACNox_Runner::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Movement
		Movement->BindInput(input);
	}

}

void ACNox_Runner::Init()
{
	// Mesh
	GetMesh()->SetRelativeLocation(FVector(0, 0, ~89));
	GetMesh()->SetRelativeRotation(FRotator(0, ~89, 0));

	// SpringArm
	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", RootComponent);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->TargetArmLength = 200;
	SpringArm->bUsePawnControlRotation = true;

	// Camera
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);
	Camera->SetRelativeLocation(FVector(~79, 30, 100));

	// MappingContext
	CHelpers::GetAsset<UInputMappingContext>(&MappingContext, TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_Runner.IMC_Runner'"));

	// Movement
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");

}
