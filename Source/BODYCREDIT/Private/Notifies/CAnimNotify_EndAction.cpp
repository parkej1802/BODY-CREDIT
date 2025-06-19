#include "Notifies/CAnimNotify_EndAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Items/Equipments/Weapons/CWeapon_DoAction.h"
#include "Items/Equipments/Weapons/CWeapon_SubAction.h"

FString UCAnimNotify_EndAction::GetNotifyName_Implementation() const
{
	return "End_DoAction";
}

void UCAnimNotify_EndAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	weapon->GetDoAction()->End_DoAction();

	// CheckFalse(weapon->IsBowMode());
	// CheckFalse(weapon->bInSubAction);
	// weapon->GetSubAction()->Pressed();

}
