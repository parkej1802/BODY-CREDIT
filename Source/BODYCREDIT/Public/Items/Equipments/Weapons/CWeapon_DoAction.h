#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Items/Equipments/Weapons/CWeapon_Structures.h"
#include "CWeapon_DoAction.generated.h"

UCLASS()
class BODYCREDIT_API UCWeapon_DoAction : public UObject
{
	GENERATED_BODY()
	
public:
	FORCEINLINE bool GetBeginAction() { return bBeginAction; }
	FORCEINLINE bool GetInAction() { return bInAction; }

public:
	UCWeapon_DoAction();

	virtual void BeginPlay
	(
		class ACWeapon_Attachment* InAttachment,
		class UCWeapon_Equipment* InEquipment,
		class ACNox* InOwner,
		const TArray<FWeaponDoActionData>& InDoActionDatas,
		const TArray<FWeaponHitData>& InHitDatas
	);
	virtual void Tick(float InDeltaTime) {}

public:
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();

public:
	UFUNCTION()
	virtual void OnBeginEquip() {}

	UFUNCTION()
	virtual void OnUnequip() {}

public:
	UFUNCTION()
	virtual void OnWeaponAttachmentBeginCollision() {}
				   
	UFUNCTION()
	virtual void OnWeaponAttachmentEndCollision() {}
	
	UFUNCTION()
	virtual void OnWeaponAttachmentBeginOverlap(class ACNox* InAttacker, AActor* InAttackCauser, class ACNox* InOther) {}
	
	UFUNCTION()
	virtual void OnWeaponAttachmentEndOverlap(class ACNox* InAttacker, class ACNox* InOther) {}

protected:
	bool bInAction;
	bool bBeginAction;

	class ACNox* OwnerCharacter;
	class UWorld* World;

	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	TArray<FWeaponDoActionData> DoActionDatas;
	TArray<FWeaponHitData> HitDatas;

};
