#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/CWeapon_DoAction.h"
#include "CWeapon_DoAction_Combo.generated.h"

UCLASS()
class BODYCREDIT_API UCWeapon_DoAction_Combo : public UCWeapon_DoAction
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

public:
	void OnWeaponAttachmentBeginOverlap(class ACNox* InAttacker, AActor* InAttackCauser, class ACNox* InOther) override;
	void OnWeaponAttachmentEndCollision() override;

private:
	int32 Index;

	bool bEnable;
	bool bExist;

private:
	TArray<class ACNox*> Hitted;

};
