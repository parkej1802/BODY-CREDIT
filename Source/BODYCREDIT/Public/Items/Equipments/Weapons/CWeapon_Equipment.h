#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Items/Equipments/Weapons/CWeapon_Structures.h"
#include "CWeapon_Equipment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponEquipmentEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponEquipmentUnequip);

UCLASS(Blueprintable)
class BODYCREDIT_API UCWeapon_Equipment : public UObject
{
	GENERATED_BODY()
	
public:
	FORCEINLINE bool GetBeginEquip() { return bBeginEquip; }
	FORCEINLINE const bool* GetEquipped() { return &bEquipped; }

public:
	void BeginPlay(class ACNox* InOwner, const FWeaponEquipmentData& InData);

public:
	UFUNCTION(BlueprintNativeEvent)
	void Equip();
	void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Begin_Equip();
	void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void End_Equip();
	void End_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Unequip();
	void Unequip_Implementation();

public:
	FWeaponEquipmentBeginEquip OnWeaponEquipmentEquip;
	FWeaponEquipmentBeginEquip OnWeaponEquipmentBeginEquip;
	FWeaponEquipmentUnequip OnWeaponEquipmentUnequip;

protected:
	UPROPERTY(BlueprintReadOnly)
	class ACNox* OwnerCharacter;

private:
	FWeaponEquipmentData Data;

private:
	class UCMovementComponent* Movement;
	class UCStateComponent* State;

private:
	bool bBeginEquip;
	bool bEquipped;

};
