#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Items/Equipments/Weapons/CWeapon_Structures.h"
#include "CWeapon_Asset.generated.h"

UCLASS()
class BODYCREDIT_API UCWeapon_Asset : public UDataAsset
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACWeapon_Attachment> WeaponAttachmentClass;

	UPROPERTY(EditAnywhere)
	FWeaponEquipmentData WeaponEquipmentData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCWeapon_Equipment> WeaponEquipmentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCWeapon_DoAction> WeaponDoActionClass;

	UPROPERTY(EditAnywhere)
	TArray<FWeaponDoActionData> WeaponDoActionDatas;

	UPROPERTY(EditAnywhere)
	TArray<FWeaponHitData> WeaponHitDatas;

	UPROPERTY(EditAnywhere)
	TArray<FWeaponDoActionData> WeaponSprintDoActionDatas;

	UPROPERTY(EditAnywhere)
	TArray<FWeaponHitData> WeaponSprintHitDatas;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCWeapon_SubAction> WeaponSubActionClass;

public:
	UCWeapon_Asset();

	void BeginPlay(class ACNox* InOwner, class UCWeapon_Data** OutWeaponData);

};
