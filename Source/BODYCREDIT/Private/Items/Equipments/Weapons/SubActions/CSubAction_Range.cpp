#include "Items/Equipments/Weapons/SubActions/CSubAction_Range.h"
#include "Global.h"
#include "characters/CNox.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Curves/CurveVector.h"
#include "Components/cstatecomponent.h"
#include "AIController.h"

void UCSubAction_Range::BeginPlay(ACNox* InOwner, ACWeapon_Attachment* InAttachment, UCWeapon_DoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);

	if (Curve)
	{
		FOnTimelineVector TimelineDelegate;
		TimelineDelegate.BindUFunction(this, "OnAiming");
		Timeline.AddInterpVector(Curve, TimelineDelegate);
		Timeline.SetPlayRate(AimingSpeed);
	}
	
}

void UCSubAction_Range::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
	
	Timeline.TickTimeline(InDeltaTime);
	
}

void UCSubAction_Range::Pressed()
{
	CheckTrue(State->IsSubActionMode());

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
	
}

void UCSubAction_Range::Released()
{
	CheckFalse(State->IsSubActionMode());

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
}

void UCSubAction_Range::OnAiming(FVector Output)
{
	if (Camera)
		Camera->SetFieldOfView(Output.X);
	
}
