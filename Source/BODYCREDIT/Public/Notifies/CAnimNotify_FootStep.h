#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_FootStep.generated.h"

UCLASS()
class BODYCREDIT_API UCAnimNotify_FootStep : public UAnimNotify
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TArray<class USoundBase*> SoundList;

	UPROPERTY(EditAnywhere)
	float Loudness = 1.0f;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000.0f;

public:
	FString GetNotifyName() const;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
