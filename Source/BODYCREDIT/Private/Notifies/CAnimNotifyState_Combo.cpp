#include "Notifies/CAnimNotifyState_Combo.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Items/Equipments/Weapons/CWeapon_DoAction_Combo.h"

FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());


	UCWeapon_DoAction_Combo* combo = Cast<UCWeapon_DoAction_Combo>(weapon->GetDoAction());
	CheckNull(combo);

	combo->EnableCombo();

}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());


	UCWeapon_DoAction_Combo* combo = Cast<UCWeapon_DoAction_Combo>(weapon->GetDoAction());
	CheckNull(combo);

	combo->DisableCombo();

}
