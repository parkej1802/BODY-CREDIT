#include "Items/Equipments/Weapons/SubActions/CSubAction_Bow.h"
#include "Global.h"
#include "Curves/CurveVector.h"
#include "Characters/CNox.h"
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

void UCSubAction_Bow::BeginPlay(ACNox* InOwner, ACWeapon_Attachment* InAttachment, UCWeapon_DoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	SpringArm = CHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera = CHelpers::GetComponent<UCameraComponent>(InOwner);

	FOnTimelineVector timeline;
	timeline.BindUFunction(this, "OnAiming");

	Timeline.AddInterpVector(Curve, timeline);
	Timeline.SetPlayRate(AimingSpeed);

	Bow = Cast<ACAttachment_Bow>(InAttachment);

	if (!!Bow)
		Bend = Bow->GetBend();

}

void UCSubAction_Bow::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	Timeline.TickTimeline(InDeltaTime);

	// 1) 공통 가드
	CheckNull(Bow);
	CheckTrue(Bow->Arrows.IsEmpty());

	// 2) LastArrow 캐싱 & 검사
	ACAddOn_Arrow* LastArrow = Bow->Arrows.Last();
	CheckNull(LastArrow);

	// 3) Movement 검사
	UProjectileMovementComponent* projectile = LastArrow->GetProjectileMovement();
	// bInAction일 땐 Movement 검사 불필요하나, 분기 간편화를 위해 미리 체크
	CheckNull(projectile);

	// 4) 가시성 결정
	bool bIsFlying   = projectile->IsActive();
	bool bShouldHide = !bInAction && !bIsFlying;

	LastArrow->SetActorHiddenInGame( bShouldHide );

	// // 4) 분기 간소화
	// if (bInAction)
	// {
	// 	LastArrow->SetActorHiddenInGame(!bInAction);
	// }
	// else
	// {
	// 	LastArrow->SetActorHiddenInGame(bInAction);
	// }

	// if (bInAction)
	// {
	// 	CheckTrue(Bow->Arrows.IsEmpty());
	// 	CheckTrue(Bow->Arrows.Last()->IsHidden());
	// 	Bow->Arrows.Last()->SetActorHiddenInGame(false);
	// }
	// else
	// {
	// 	// SubAction중이 아니거나 Arrows가 비어 있거나 마지막 화살의 활성화 상태일 경우
	// 	CheckTrue(Bow->Arrows.IsEmpty());
	// 	CheckTrue(Bow->Arrows.Last()->GetProjectileMovement()->IsActive());
	// 	
	// 	Bow->Arrows.Last()->SetActorHiddenInGame(true);
	// }

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
