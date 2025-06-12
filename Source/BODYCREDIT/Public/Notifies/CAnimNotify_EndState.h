#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/CStateComponent.h"
#include "CAnimNotify_EndState.generated.h"

UCLASS()
class BODYCREDIT_API UCAnimNotify_EndState : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Type")
	EStateType StateType;
	
public:
	FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

};
