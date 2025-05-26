#include "Notifies/CAnimNotify_ChangeSocket.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Items/Equipments/Weapons/CWeapon_Attachment.h"

FString UCAnimNotify_ChangeSocket::GetNotifyName_Implementation() const
{
	return "ChangeSocket";
}

void UCAnimNotify_ChangeSocket::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetAttachment());

	if (SocketName.IsValid())
		weapon->GetAttachment()->AttachTo(SocketName);

}
