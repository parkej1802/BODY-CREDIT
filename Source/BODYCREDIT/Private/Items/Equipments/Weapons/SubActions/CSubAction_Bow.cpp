#include "Items/Equipments/Weapons/SubActions/CSubAction_Bow.h"
#include "Global.h"
#include "Curves/CurveVector.h"
#include "Characters/CNox_Runner.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AIController.h"
#include "Components/CStateComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Implicit/SparseNarrowBandMeshSDF.h"
#include "Items/Equipments/Weapons/AddOns/CAddOn_Arrow.h"
#include "Items/Equipments/Weapons/Attachments/CAttachment_Bow.h"
#include "Items/Equipments/Weapons/DoActions/CDoAction_Bow.h"

UCSubAction_Bow::UCSubAction_Bow()
{
	CHelpers::GetAsset<UCurveVector>(&Curve, "/Script/Engine.CurveVector'/Game/Items/Equipments/Weapons/Bow/Curve_Aiming.Curve_Aiming'");
}

void UCSubAction_Bow::BeginPlay(ACNox * InOwner, ACWeapon_Attachment * InAttachment, UCWeapon_DoAction * InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);
	
	OwnerCharacter = Cast<ACNox_Runner>(InOwner);

	Bow = Cast<ACAttachment_Bow>(InAttachment);

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);

	FOnTimelineVector timeline;
	timeline.BindUFunction(this, "OnAiming");

	Timeline.AddInterpVector(Curve, timeline);
	Timeline.SetPlayRate(AimingSpeed);


	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	
	if (!!bow)
		Bend = bow->GetBend();
}

void UCSubAction_Bow::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	Timeline.TickTimeline(InDeltaTime);
}

void UCSubAction_Bow::OnAiming(FVector Output)
{
	Camera->FieldOfView = Output.X;
	
	if (!!Bend)
		*Bend = Output.Y;	
}

void UCSubAction_Bow::Pressed()
{
	CheckTrue(State->IsSubActionMode());

	Bow->Arrows.Last()->SetActorHiddenInGame(false);

	if (!!Owner->GetController<AAIController>())
	{
		Super::Pressed();
		State->OnSubActionMode();

		return;
	}

	CheckNull(SpringArm);
	CheckNull(Camera);

	Super::Pressed();

	State->OnSubActionMode();
	
	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();

	SpringArm->TargetArmLength = AimData.TargetArmLength;
	SpringArm->SocketOffset = AimData.SocketOffset;
	SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	Timeline.PlayFromStart();
}

void UCSubAction_Bow::Released()
{
	CheckFalse(State->IsSubActionMode());

	if (!Bow->Arrows.Last()->GetProjectileMovement()->IsActive())
		Bow->Arrows.Last()->SetActorHiddenInGame(false);

	if (!!Owner->GetController<AAIController>())
	{
		Super::Released();
		State->OffSubActionMode();
		
		return;
	}
	

	CheckNull(SpringArm);
	CheckNull(Camera);

	Super::Released();

	State->OffSubActionMode();

	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	Timeline.ReverseFromEnd();
}