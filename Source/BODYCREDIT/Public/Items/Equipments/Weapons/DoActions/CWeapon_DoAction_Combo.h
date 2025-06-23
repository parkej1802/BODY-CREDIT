#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/CWeapon_DoAction.h"
#include "CWeapon_DoAction_Combo.generated.h"

UCLASS(blueprintable)
class BODYCREDIT_API UCWeapon_DoAction_Combo : public UCWeapon_DoAction
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Combo")
	class UParticleSystem* HitVFX;
	
public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

public:
	void OnWeaponAttachmentBeginOverlap(class ACNox* InAttacker, AActor* InAttackCauser, class ACNox* InOther, const struct FHitResult& HitResult) override;
	void OnWeaponAttachmentEndCollision() override;

protected:
	int32 Index;
	
private:
	bool bEnable;
	bool bExist;

private:
	TArray<class ACNox*> Hitted;

};
