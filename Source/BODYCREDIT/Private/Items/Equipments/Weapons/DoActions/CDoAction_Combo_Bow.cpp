#include "Items/Equipments/Weapons/DoActions/CDoAction_Combo_Bow.h"
#include "Global.h"
#include "Items/Equipments/Weapons/CWeapon_Attachment.h"
#include "Items/Equipments/Weapons/Attachments/CAttachment_Bow.h"
#include "Components/PoseableMeshComponent.h"
#include "Items/Equipments/Weapons/CWeapon_Equipment.h"
#include "Components/CStateComponent.h"
#include "Characters/CNox.h"
#include "Items/Equipments/Weapons/AddOns/CAddOn_Arrow.h"

UCDoAction_Combo_Bow::UCDoAction_Combo_Bow()
{
	
}

void UCDoAction_Combo_Bow::BeginPlay(class ACWeapon_Attachment* InAttachment, class UCWeapon_Equipment* InEquipment,
	class ACNox* InOwner, const TArray<FWeaponDoActionData>& InDoActionDatas, const TArray<FWeaponHitData>& InHitDatas,
	const TArray<FWeaponDoActionData>& InSprintDoActionDatas, const TArray<FWeaponHitData>& InSprintHitDatas)
{
	Super::BeginPlay(InAttachment, InEquipment, InOwner, InDoActionDatas, InHitDatas, InSprintDoActionDatas,
	                 InSprintHitDatas);

	SkeletalMesh = CHelpers::GetComponent<USkeletalMeshComponent>(InAttachment);
	PoseableMesh = CHelpers::GetComponent<UPoseableMeshComponent>(InAttachment);

	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	Bending = bow->GetBend();

	OriginLocation = PoseableMesh->GetBoneLocationByName("ArrowBase", EBoneSpaces::ComponentSpace);

	bEquipped = InEquipment->GetEquipped();
	
}

void UCDoAction_Combo_Bow::DoAction()
{
	CheckTrue(bInAction);
	
	Super::DoAction();
	
	bInAction = true;
	
	// CheckTrue(DoActionDatas.Num() < 1);
	// CheckFalse(State->IsIdleMode());
	//
	// if (CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter)->IsBowSubActionActive())
	// {
	// 	Super::DoAction();
	// 	DoActionDatas[Index].DoAction(OwnerCharacter);
	// }
	// else DoActionDatas[0].DoAction(OwnerCharacter);
	
}
void UCDoAction_Combo_Bow::Begin_DoAction()
{
	Super::Begin_DoAction();
	
	bAttachedString = false;
	
	*Bending = 0;
	PoseableMesh->SetBoneLocationByName("ArrowBase", OriginLocation, EBoneSpaces::ComponentSpace);
	
	CheckNull(ArrowClass);

	if (ACAddOn_Arrow* arrow = GetAttachedArrow())
	{
		arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

		arrow->OnHit.AddDynamic(this, &UCDoAction_Combo_Bow::OnArrowHit);
		arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Combo_Bow::OnArrowEndPlay);
	
		FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
		arrow->Shoot(forward);
	}
	
}

void UCDoAction_Combo_Bow::End_DoAction()
{
	Super::End_DoAction();

	bInAction = false;

	CreateArrow();
}

void UCDoAction_Combo_Bow::OnBeginEquip()
{
	Super::OnBeginEquip();

	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CreateArrow();
}

void UCDoAction_Combo_Bow::OnUnequip()
{
	Super::OnUnequip();

	*Bending = 0;
	OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);
	
	for (int32 i = Arrows.Num() - 1; i >= 0; i--)
	{
		if (!!Arrows[i]->GetAttachParentActor())
			Arrows[i]->Destroy();
	}
}

void UCDoAction_Combo_Bow::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
	
	// PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

	bool bCheck = true;
	bCheck &= (*bEquipped == true);
	bCheck &= (bBeginAction == false);
	bCheck &= (bAttachedString == true);

	CheckFalse(bCheck);

	FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_String");
	PoseableMesh->SetBoneLocationByName("ArrowBase", handLocation, EBoneSpaces::WorldSpace);
}

void UCDoAction_Combo_Bow::End_BowString()
{
	*Bending = 100;
	bAttachedString = true;
}

void UCDoAction_Combo_Bow::CreateArrow()
{
	if (World->bIsTearingDown == true)
		return;
	
	FTransform transform;
	ACAddOn_Arrow* arrow = World->SpawnActorDeferred<ACAddOn_Arrow>(ArrowClass, transform, NULL, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	CheckNull(arrow);

	arrow->AddIgnoreActor(OwnerCharacter);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Arrow");

	Arrows.Add(arrow);
	UGameplayStatics::FinishSpawningActor(arrow, transform);
}

class ACAddOn_Arrow* UCDoAction_Combo_Bow::GetAttachedArrow()
{
	for (ACAddOn_Arrow* projectile : Arrows)
	{
		if (!!projectile->GetAttachParentActor())
			return projectile;
	}

	return nullptr;
}

void UCDoAction_Combo_Bow::OnArrowHit(class AActor* InCauser, class ACNox* InOtherCharacter)
{
	CheckFalse(HitDatas.Num() > 0);

	HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);
}

void UCDoAction_Combo_Bow::OnArrowEndPlay(class ACAddOn_Arrow* InDestroyer)
{
	Arrows.Remove(InDestroyer);
	
}
