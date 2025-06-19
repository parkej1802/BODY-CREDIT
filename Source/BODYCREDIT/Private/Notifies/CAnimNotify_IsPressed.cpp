#include "Notifies/CAnimNotify_IsPressed.h"
#include "Global.h"
#include "Characters/CNox_Runner.h"
#include "Components/CWeaponComponent.h"
#include "Animation/AnimInstance.h"
#include "Items/Equipments/Weapons/CWeapon_SubAction.h"

FString UCAnimNotify_IsPressed::GetNotifyName() const
{
	return "IsPressed";
	
}

void UCAnimNotify_IsPressed::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACNox_Runner* runner = Cast<ACNox_Runner>(MeshComp->GetOwner());
	CheckNull(runner);

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(runner);
	CheckNull(weapon);

	CheckFalse(weapon->IsBowMode());
	CheckTrue(weapon->bInSubAction);

	weapon->GetSubAction()->Released();

}
