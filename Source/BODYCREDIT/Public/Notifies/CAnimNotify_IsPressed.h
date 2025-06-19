#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_IsPressed.generated.h"

UCLASS()
class BODYCREDIT_API UCAnimNotify_IsPressed : public UAnimNotify
{
	GENERATED_BODY()

public:
	FString GetNotifyName() const;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference);
	
};
