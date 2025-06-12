#include "Notifies/CAnimNotify_EndState.h"
#include "Global.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation() const
{
	return "EndState";

}

void UCAnimNotify_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());



}
