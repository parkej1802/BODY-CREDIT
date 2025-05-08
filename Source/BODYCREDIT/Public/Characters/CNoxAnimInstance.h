#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CNoxAnimInstance.generated.h"

UCLASS()
class BODYCREDIT_API UCNoxAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeBeginPlay() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY()
	class ACNox* OwnerCharacter;

protected:
	UPROPERTY(BlueprintReadOnly)
	float Direction;

	UPROPERTY(BlueprintReadOnly)
	float Speed;

	UPROPERTY(BlueprintReadOnly)
	bool bFalling;

private:
	FRotator PrevRotation;

};
