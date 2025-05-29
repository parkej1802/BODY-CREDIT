#include "Items/Equipments/Weapons/CDoAction_Bow.h"
#include "Global.h"
#include "Characters/CNox.h"
//#include "Items/Equipments/Weapons/CAttachment_Bow.h"
#include "Items/Equipments/Weapons/CWeapon_Equipment.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"

UCDoAction_Bow::UCDoAction_Bow()
{

}

void UCDoAction_Bow::BeginPlay(ACWeapon_Attachment* InWeaponAttachment, UCWeapon_Equipment* InEquipment, ACNox* InOwner, const TArray<FWeaponDoActionData>& InDoActionData, const TArray<FWeaponHitData>& InHitData)
{
	Super::BeginPlay(InWeaponAttachment, InEquipment, InOwner, InDoActionData, InHitData);

	//SkeletalMesh = CHelpers::GetComponent<USkeletalMeshComponent>(InWeaponAttachment);
	//PoseableMesh = CHelpers::GetComponent<UPoseableMeshComponent>(InWeaponAttachment);

	//ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	//Bending = bow->GetBend();

	//OriginLocation = PoseableMesh->GetBoneLocationByName("bow_string_mid", EBoneSpaces::ComponentSpace);

	//bEquipped = InEquipment->GetEquipped();

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
	//PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);

	//CheckNull(ArrowClass);

	//ACArrow* arrow = GetAttachedArrow();
	//arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

	//arrow->OnHit.AddDynamic(this, &UCDoAction_Bow::OnArrowHit);
	//arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::OnArrowEndPlay);

	//FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();
	//arrow->Shoot(forward);

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

	//*Bending = 0;
	//OwnerCharacter->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//PoseableMesh->SetBoneLocationByName("bow_string_mid", OriginLocation, EBoneSpaces::ComponentSpace);


	//for (int32 i = Arrows.Num() - 1; i >= 0; i--)
	//{
	//	if (!!Arrows[i]->GetAttachParentActor())
	//		Arrows[i]->Destroy();
	//}

}

void UCDoAction_Bow::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	//PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

	//bool bCheck = true;
	//bCheck &= (*bEquipped == true);
	//bCheck &= (bBeginAction == false);
	//bCheck &= (bAttachedString == true);

	//CheckFalse(bCheck);

	//FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right");
	//PoseableMesh->SetBoneLocationByName("bow_string_mid", handLocation, EBoneSpaces::WorldSpace);

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

	//FTransform transform;
	//ACArrow* arrow = World->SpawnActorDeferred<ACArrow>(ArrowClass, transform, NULL, NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	//CheckNull(arrow);

	//arrow->AddIgnoreActor(OwnerCharacter);

	//FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	//arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Right_Arrow");

	//Arrows.Add(arrow);
	//UGameplayStatics::FinishSpawningActor(arrow, transform);

}

ACArrow* UCDoAction_Bow::GetAttachedArrow()
{
	//for (ACArrow* projectile : Arrows)
	//{
	//	if (!!projectile->GetAttachParentActor())
	//		return projectile;
	//}

	return nullptr;

}

void UCDoAction_Bow::OnArrowHit(AActor* InCauser, ACNox* InOtherCharacter)
{
	CheckFalse(HitDatas.Num() > 0);

	HitDatas[0].SendDamage(OwnerCharacter, InCauser, InOtherCharacter);

}

//void UCDoAction_Bow::OnArrowEndPlay(ACArrow* InDestroyer)
//{
//	Arrows.Remove(InDestroyer);
//
//}
