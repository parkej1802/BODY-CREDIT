#include "Items/Equipments/Weapons/DoActions/CDoAction_Bow.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "Characters/CNox.h"
#include "Components/PoseableMeshComponent.h"
#include "Items/Equipments/Weapons/Attachments/CAttachment_Bow.h"
#include "Items/Equipments/Weapons/CWeapon_Equipment.h"
#include "Items/Equipments/Weapons/AddOns/CAddOn_Arrow.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

// UCDoAction_Bow::UCDoAction_Bow()
// {
// }
//
// void UCDoAction_Bow::BeginPlay
// (
// 	ACWeapon_Attachment* InWeaponAttachment,
// 	UCWeapon_Equipment* InEquipment,
// 	ACNox* InOwner,
// 	const TArray<FWeaponDoActionData>& InDoActionDatas,
// 	const TArray<FWeaponHitData>& InHitDatas,
// 	const TArray<FWeaponDoActionData>& InSprintDoActionDatas,
// 	const TArray<FWeaponHitData>& InSprintHitDatas
// 	)
// {
// 	Super::BeginPlay(InWeaponAttachment, InEquipment, InOwner, InDoActionDatas, InHitDatas, InSprintDoActionDatas, InSprintHitDatas);
//
// 	SkeletalMesh = CHelpers::GetComponent<USkeletalMeshComponent>(InWeaponAttachment);
// 	PoseableMesh = CHelpers::GetComponent<UPoseableMeshComponent>(InWeaponAttachment);
// 	Bow = Cast<ACAttachment_Bow>(InWeaponAttachment);
// 	Bending = Bow->GetBend();
//
// 	OriginLocation = PoseableMesh->GetBoneLocationByName("ArrowBase", EBoneSpaces::ComponentSpace);
// 	bEquipped = InEquipment->GetEquipped();
//
// 	CreateArrow();
// }
//
// void UCDoAction_Bow::Tick(float InDeltaTime)
// {
// 	Super::Tick(InDeltaTime);
//
// 	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
//
// 	if (!bIsCharging || !(*bEquipped) || !bAttachedString)
// 		return;
// 	
// 	ChargeTime += InDeltaTime;
// 	UpdateChargeStage();
// 	CLog::Print(static_cast<int32>(ChargeStage));
// 	// UE_LOG(LogTemp, Log, TEXT("ChargeStage: %d"), static_cast<int32>(ChargeStage));
// 	
// 	FVector HandLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_String");
// 	PoseableMesh->SetBoneLocationByName("ArrowBase", HandLocation, EBoneSpaces::WorldSpace);
// }
//
// void UCDoAction_Bow::Pressed()
// {
// 	if (!State->IsIdleMode() || !State->IsSubActionMode())
// 		return;
//
// 	Super::DoAction();
// 	StartCharging();
// }
//
// void UCDoAction_Bow::Released()
// {
// 	ReleaseCharging();
// }
//
// void UCDoAction_Bow::End_BowString()
// {
// 	*Bending = 100;
// 	bAttachedString = true;
//
// 	CreateArrow();
// }
//
// void UCDoAction_Bow::StartCharging()
// {
// 	ChargeTime = 0.0f;
// 	bIsCharging = true;
// 	ChargeStage = EArrowChargeStage::Stage1;
//
// 	// GetWorld()->GetTimerManager().SetTimer(
// 	// 	ChargeTimerHandle,
// 	// 	[this]()
// 	// 	{
// 	// 		ChargeTime += World->GetDeltaSeconds();
// 	// 		UpdateChargeStage();
// 	// 	},
// 	// 	0.05f, true);
// }
//
// void UCDoAction_Bow::UpdateChargeStage()
// {
// 	if (ChargeTime >= 3.5f)
// 	{
// 		ChargeStage = EArrowChargeStage::Stage3;
// 	}
// 	else if (ChargeTime >= 1.5f)
// 	{
// 		ChargeStage = EArrowChargeStage::Stage2;
// 	}
// 	else
// 	{
// 		ChargeStage = EArrowChargeStage::Stage1;
// 	}
// }
//
// void UCDoAction_Bow::ReleaseCharging()
// {
// 	if (!bIsCharging) return;
//
// 	bIsCharging = false;
// 	// GetWorld()->GetTimerManager().ClearTimer(ChargeTimerHandle);
//
// 	Super::Begin_DoAction();
//
// 	ACAddOn_Arrow* Arrow = GetAttachedArrow();
// 	if (!Arrow) return;
//
// 	Arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
// 	Arrow->OnHit.AddDynamic(this, &UCDoAction_Bow::OnArrowHit);
// 	Arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::OnArrowEndPlay);
//
// 	FVector Forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
// 	float Power = 1.0f;
//
// 	switch (ChargeStage)
// 	{
// 		case EArrowChargeStage::Stage2: Power = 2.2; break;
// 		case EArrowChargeStage::Stage3: Power = 3.5; break;
// 		default: break;
// 	}
//
// 	Arrow->Shoot(Forward);
//
// 	if (!DoActionDatas.IsEmpty())
// 		DoActionDatas[0].DoAction(OwnerCharacter);
// 	
// }
//
// void UCDoAction_Bow::CreateArrow()
// {
// 	FTransform Transform;
// 	ACAddOn_Arrow* Arrow = World->SpawnActorDeferred<ACAddOn_Arrow>(ArrowClass, Transform, Bow->GetOwner(), nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
// 	CheckNull(Arrow);
//
// 	Arrow->AddIgnoreActor(OwnerCharacter);
//
// 	FAttachmentTransformRules Rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
// 	Arrow->AttachToComponent(OwnerCharacter->GetMesh(), Rule, "Hand_Bow_Arrow");
// 	Arrow->SetHidden(true);
//
// 	Bow->Arrows.Add(Arrow);
// 	UGameplayStatics::FinishSpawningActor(Arrow, Transform);
// }
//
// ACAddOn_Arrow* UCDoAction_Bow::GetAttachedArrow()
// {
// 	for (ACAddOn_Arrow* Projectile : Bow->Arrows)
// 	{
// 		if (!!Projectile->GetAttachParentActor())
// 			return Projectile;
// 	}
//
// 	return nullptr;
// }
//
// void UCDoAction_Bow::OnArrowHit(AActor* InCauser, ACNox* InOtherCharacter)
// {
// 	CheckFalse(HitDatas.Num() > 0);
// 	HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);
// }
//
// void UCDoAction_Bow::OnArrowEndPlay(ACAddOn_Arrow* InDestroyer)
// {
// 	Bow->Arrows.Remove(InDestroyer);
// }

UCDoAction_Bow::UCDoAction_Bow()
{

}

void UCDoAction_Bow::BeginPlay(class ACWeapon_Attachment* InAttachment, class UCWeapon_Equipment* InEquipment, class ACNox* InOwner, const TArray<FWeaponDoActionData>& InDoActionDatas, const TArray<FWeaponHitData>& InHitDatas, const TArray<FWeaponDoActionData>& InSprintDoActionDatas, const TArray<FWeaponHitData>& InSprintHitDatas)
{
	Super::BeginPlay(InAttachment, InEquipment, InOwner, InDoActionDatas, InHitDatas, InSprintDoActionDatas, InSprintHitDatas);

	SkeletalMesh = CHelpers::GetComponent<USkeletalMeshComponent>(InAttachment);
	PoseableMesh = CHelpers::GetComponent<UPoseableMeshComponent>(InAttachment);

	Bow = Cast<ACAttachment_Bow>(InAttachment);
	Bending = Bow->GetBend();

	OriginLocation = PoseableMesh->GetBoneLocationByName("ArrowBase", EBoneSpaces::ComponentSpace);

	bEquipped = InEquipment->GetEquipped();
}

void UCDoAction_Bow::DoAction()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(State->IsSubActionMode());

	Super::DoAction();

	if (!DoActionDatas.IsEmpty())
		DoActionDatas[0].DoAction(OwnerCharacter);

}

void UCDoAction_Bow::Begin_DoAction()
{
	Super::Begin_DoAction();

	bAttachedString = false;

	*Bending = 0;
	PoseableMesh->SetBoneLocationByName("ArrowBase", OriginLocation, EBoneSpaces::ComponentSpace);

	CheckNull(ArrowClass);

	ACAddOn_Arrow* arrow = GetAttachedArrow();
	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	arrow->OnHit.AddDynamic(this, &UCDoAction_Bow::OnArrowHit);
	arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::OnArrowEndPlay);

	// 1) 컨트롤러(카메라) 정보 얻기
	AController* OwnerController = OwnerCharacter->GetController();
	if (OwnerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Controller for Bow Owner"));
		return;
	}

	FVector CameraLocation;
	FRotator CameraRotation;
	OwnerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

	// 2) 라인 트레이스
	const float TraceDistance = 10000.0f;
	FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * TraceDistance);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwnerCharacter);

	bool bHit = GetWorld()->LineTraceSingleByChannel
	(
		Hit,
		CameraLocation,
		TraceEnd,
		ECC_Visibility,
		Params
	);

	// 3) 목표 지점 결정
	FVector AimPoint = bHit ? Hit.Location : TraceEnd;
	FVector forward = (AimPoint - OwnerCharacter->GetActorLocation()).GetSafeNormal();
	
	// FVector forward = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter)->GetForwardVector();
	arrow->Shoot(forward);

}

void UCDoAction_Bow::End_DoAction()
{
	Super::End_DoAction();

	CreateArrow();

}

void UCDoAction_Bow::OnBeginEquip()
{
	Super::OnBeginEquip();

	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CreateArrow();

}

void UCDoAction_Bow::OnUnequip()
{
	Super::OnUnequip();

	*Bending = 0;
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	PoseableMesh->SetBoneLocationByName("ArrowBase", OriginLocation, EBoneSpaces::ComponentSpace);

	for (int32 i = Bow->Arrows.Num() - 1; i >= 0; i--)
	{
		if (!!Bow->Arrows[i]->GetAttachParentActor())
			Bow->Arrows[i]->Destroy();
	}

}

void UCDoAction_Bow::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
	
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

	bool bCheck = true;
	bCheck &= (*bEquipped == true);
	bCheck &= (bBeginAction == false);
	bCheck &= (bAttachedString == true);

	CheckFalse(bCheck);

	FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_String");
	PoseableMesh->SetBoneLocationByName("ArrowBase", handLocation, EBoneSpaces::WorldSpace);

}

void UCDoAction_Bow::End_BowString()
{
	*Bending = 100;
	bAttachedString = true;

}

void UCDoAction_Bow::CreateArrow()
{
	if (World->bIsTearingDown == true)
		return;

	FTransform transform;
	ACAddOn_Arrow* arrow = World->SpawnActorDeferred<ACAddOn_Arrow>(ArrowClass, transform, Bow->GetOwner(), NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	CheckNull(arrow);

	arrow->AddIgnoreActor(OwnerCharacter);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Arrow");
	arrow->SetHidden(true);

	Bow->Arrows.Add(arrow);
	UGameplayStatics::FinishSpawningActor(arrow, transform);

}

ACAddOn_Arrow* UCDoAction_Bow::GetAttachedArrow()
{
	for (ACAddOn_Arrow* projectile : Bow->Arrows)
	{
		if (!!projectile->GetAttachParentActor())
			return projectile;
	}

	return nullptr;

}

void UCDoAction_Bow::OnArrowHit(AActor* InCauser, ACNox* InOtherCharacter)
{
	CheckFalse(HitDatas.Num() > 0);

	HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);

}

void UCDoAction_Bow::OnArrowEndPlay(ACAddOn_Arrow* InDestroyer)
{
	Bow->Arrows.Remove(InDestroyer);

}
