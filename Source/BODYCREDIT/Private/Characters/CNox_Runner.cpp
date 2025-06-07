#include "Characters/CNox_Runner.h"
#include "Global.h"
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
#include "Inventory/AC_InventoryComponent.h"
#include "Components/CNoxHPComponent.h"
#include "Games/CMainGM.h"

#include "Data/CMemoryData.h"
#include "Inventory/AC_EquipComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Session/NetGameInstance.h"
#include "GameState_BodyCredit.h"
#include "Inventory/AC_MarketComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Characters/CNox_Controller.h"

ACNox_Runner::ACNox_Runner()
{
	PrimaryActorTick.bCanEverTick = true;

	Init();
	CHelpers::CreateActorComponent<UCNoxHPComponent>(this, &HPComp, "HPComp");

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

	Movement->EnableControlRotation();

	UNetGameInstance* GI = Cast<UNetGameInstance>(GetGameInstance());
	if (GI && EquipComp)
	{
		if (GEngine)
		{
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
	}

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
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ACNox_Runner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITOR
	const FVector Location = GetActorLocation();

	// 1. Actor Rotation (청록색)
	const FRotator ActorRot = GetActorRotation();
	const FVector ActorDir = ActorRot.Vector() * 300.0f;
	DrawDebugDirectionalArrow(GetWorld(), Location, Location + ActorDir, 100.f, FColor::Cyan, false, -1.f, 0, 2.f);
	DrawDebugString(GetWorld(), Location + ActorDir + FVector(0, 0, 20.f),
	                FString::Printf(TEXT("ActorYaw: %.1f"), ActorRot.Yaw), nullptr, FColor::Cyan, 0.f, true);

	// 2. Control Rotation (노란색)
	const FRotator ControlRot = GetControlRotation();
	const FVector ControlDir = ControlRot.Vector() * 300.0f;
	DrawDebugDirectionalArrow(GetWorld(), Location, Location + ControlDir, 100.f, FColor::Yellow, false, -1.f, 0, 2.f);
	DrawDebugString(GetWorld(), Location + ControlDir + FVector(0, 0, 40.f),
	                FString::Printf(TEXT("ControlYaw: %.1f"), ControlRot.Yaw), nullptr, FColor::Yellow, 0.f, true);

	// 3. Velocity Rotation (빨간색)
	if (!GetVelocity().IsNearlyZero())
	{
		const FRotator VelocityRot = GetVelocity().ToOrientationRotator();
		const FVector VelocityDir = VelocityRot.Vector() * 300.0f;
		DrawDebugDirectionalArrow(GetWorld(), Location, Location + VelocityDir, 100.f, FColor::Red, false, -1.f, 0,
		                          2.f);
		DrawDebugString(GetWorld(), Location + VelocityDir + FVector(0, 0, 60.f),
		                FString::Printf(TEXT("VelocityYaw: %.1f"), VelocityRot.Yaw), nullptr, FColor::Red, 0.f, true);
	}
#endif
}

void ACNox_Runner::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// MappingContext
	if (ACNox_Controller* PlayerController = Cast<ACNox_Controller>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
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

		// Weapon
		Weapon->BindInput(input);
	}
}

void ACNox_Runner::Init()
{
	{ // Modular Character Mesh
		// Head
		GetMesh()->SetRelativeLocation(FVector(0, 0, ~84));
		GetMesh()->SetRelativeRotation(FRotator(0, ~89, 0));

		// Hair
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Hair, "Hair", GetMesh());

		// UpperBody
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &UpperBody, "UpperBody", GetMesh());

		// UpperBody
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Clothes, "Clothes", GetMesh());

		// Arms
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Arms, "Arms", GetMesh());

		// LowerBody
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &LowerBody, "LowerBody", GetMesh());

		// Foot
		CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Foot, "Foot", GetMesh());
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

	//// FPSCamera
	//CHelpers::CreateComponent<UCameraComponent>(this, &FPSCamera, "FPSCamera", GetMesh(), FName("FPSCamera"));
	//FPSCamera->bUsePawnControlRotation = false;

	// MappingContext
	CHelpers::GetAsset<UInputMappingContext>(&MappingContext,
	                                         TEXT(
		                                         "/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_Runner.IMC_Runner'"));

	// Trajectory
	//CHelpers::CreateActorComponent<UCharacterTrajectoryComponent>(this, &Trajectory, "Trajectory");

	// State
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");

	// Movement
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");

	// Weapon
	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");

	// Zoom
	CHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");

}

void ACNox_Runner::MakeMemoryPiece()
{
	MainGM->RegisterMemoryFromPlayer(this, EMemoryTriggerType::Intrusion);
}

UItemObject* ACNox_Runner::CreateItemFromData(const FItemSaveData& Data)
{
	UItemObject* NewItem = NewObject<UItemObject>();
	NewItem->ImportData(Data);
	return NewItem;
}

void ACNox_Runner::CacheDefaultSkeletalMeshes()
{
	//DefaultMeshes.Add(EPlayerPart::Head, GetMesh()->SkeletalMesh);
	//DefaultMeshes.Add(EPlayerPart::Body, UpperBody->SkeletalMesh);
	//DefaultMeshes.Add(EPlayerPart::Arm, Arms->SkeletalMesh);
	//DefaultMeshes.Add(EPlayerPart::Leg, LowerBody->SkeletalMesh);
}
