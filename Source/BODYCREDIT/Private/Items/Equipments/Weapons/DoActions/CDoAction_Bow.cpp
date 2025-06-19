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

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, CameraLocation, TraceEnd, ECC_Visibility, Params);

	// 3) 목표 지점 결정
	FVector AimPoint = bHit ? Hit.Location : TraceEnd;
	FVector forward = (AimPoint - OwnerCharacter->GetActorLocation()).GetSafeNormal();

	// FVector forward = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter)->GetForwardVector();
	arrow->Shoot(forward);
	
	// FVector TraceStart;
	// FRotator TraceRot;
	// OwnerCharacter->GetController()->GetPlayerViewPoint(TraceStart, TraceRot);
	//
	// FVector TraceDir = TraceRot.Vector();
	// FVector TraceEnd = TraceStart + (TraceDir * 10000.0f);
	//
	// FHitResult HitResult;
	// FCollisionQueryParams TraceParams(
	// 	NAME_None,
	// 	true,
	// 	OwnerCharacter
	// );
	// bool bHit = GetWorld()->LineTraceSingleByChannel(
	// 	HitResult,
	// 	TraceStart,
	// 	TraceEnd,
	// 	ECC_Visibility,
	// 	TraceParams
	// );
	//
	// FVector ImpactPoint = bHit
	// 	? HitResult.ImpactPoint
	// 	: TraceEnd;
	//
	// // 5. 화살 발사
	// FVector LaunchDir = (ImpactPoint - arrow->GetTransform().GetLocation()).GetSafeNormal();
	// arrow->Shoot(LaunchDir);

	// // 이전 방식: 컨트롤러 회전으로 단순 발사 방향 계산
	// // FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
	//
	// UCameraComponent* camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	// FVector TraceStart = camera->GetComponentLocation();
	// FVector TraceEnd = TraceStart + camera->GetForwardVector() * 10000;
	//
	// FHitResult result;
	// FCollisionQueryParams params;
	// params.bDebugQuery = true;
	// GetWorld()->LineTraceSingleByChannel(result, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, FCollisionQueryParams(FName(TEXT("")), true, OwnerCharacter));
	// FVector LaunchDirection = result.ImpactPoint - Bow->GetTransform().GetLocation();
	// DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, true, 10, 0, 10.0f);
	//
	// // 수정된 발사 호출
	// arrow->Shoot(LaunchDirection.GetSafeNormal());
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

	for (int32 i = Arrows.Num() - 1; i >= 0; i--)
	{
		if (!!Arrows[i]->GetAttachParentActor()) Arrows[i]->Destroy();
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

	FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Arrow");
	PoseableMesh->SetBoneLocationByName("ArrowBase", handLocation, EBoneSpaces::WorldSpace);
}

void UCDoAction_Bow::End_BowString()
{
	*Bending = 100;
	bAttachedString = true;
}

void UCDoAction_Bow::CreateArrow()
{
	if (World->bIsTearingDown == true) return;


	FTransform transform;
	ACAddOn_Arrow* arrow = World->SpawnActorDeferred<ACAddOn_Arrow>(ArrowClass, transform, NULL, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	CheckNull(arrow);

	arrow->AddIgnoreActor(OwnerCharacter);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Arrow");

	Arrows.Add(arrow);
	UGameplayStatics::FinishSpawningActor(arrow, transform);
}

ACAddOn_Arrow* UCDoAction_Bow::GetAttachedArrow()
{
	for (ACAddOn_Arrow* projectile : Arrows)
	{
		if (!!projectile->GetAttachParentActor()) return projectile;
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
	Arrows.Remove(InDestroyer);
}
