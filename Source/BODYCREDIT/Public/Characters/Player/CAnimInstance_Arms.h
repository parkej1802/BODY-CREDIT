#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CGunComponent.h"
#include "CAnimInstance_Arms.generated.h"

UCLASS()
class BODYCREDIT_API UCAnimInstance_Arms : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	EGunType WeaponType = EGunType::Max;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	FTransform LeftHandTransform;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
	void OnWeaponTypeChanged(EGunType InPrevType, EGunType InNewType);

private:
	class ACNox_Player* OwnerCharacter;
	class UCGunComponent* Weapon;
};
