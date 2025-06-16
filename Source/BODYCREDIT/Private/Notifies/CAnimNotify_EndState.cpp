#include "Notifies/CAnimNotify_EndState.h"
#include "Global.h"
#include "interfaces/ICharacter.h"
#include "Characters/CNox_Runner.h"
#include "Components/CStateComponent.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation() const
{
	return "EndState";

}

void UCAnimNotify_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	if (ACNox_Runner* runner = Cast<ACNox_Runner>(MeshComp->GetOwner()))
	{
		if (UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(runner))
		{
			if (state->IsHittedMode())
			{
				runner->End_Hitted();
			}
			else if (state->IsAvoidMode())
			{
				runner->End_Avoid();	
			}
			else if (state->IsDeadMode())
			{
				runner->End_Dead();
			}
		}
	}

}
