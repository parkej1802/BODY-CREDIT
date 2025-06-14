#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeapon_Data.generated.h"

UCLASS()
class BODYCREDIT_API UCWeapon_Data : public UObject
{
	GENERATED_BODY()

private:
	friend class UCWeapon_Asset;

public:
	FORCEINLINE class ACWeapon_Attachment* GetAttachment() { return WeaponAttachment; }
	FORCEINLINE class UCWeapon_Equipment* GetEquipment() { return WeaponEquipment; }
	FORCEINLINE class UCWeapon_DoAction* GetDoAction() { return WeaponDoAction; }
	FORCEINLINE class UCWeapon_DoAction* GetSprintDoAction() { return WeaponSprintDoAction; }
	FORCEINLINE class UCWeapon_SubAction* GetSubAction() { return WeaponSubAction; }

private:
	UPROPERTY()
	class ACWeapon_Attachment* WeaponAttachment;

	UPROPERTY()
	class UCWeapon_Equipment* WeaponEquipment;

	UPROPERTY()
	class UCWeapon_DoAction* WeaponDoAction;

	UPROPERTY()
	class UCWeapon_DoAction* WeaponSprintDoAction;

	UPROPERTY()
	class UCWeapon_SubAction* WeaponSubAction;

};
