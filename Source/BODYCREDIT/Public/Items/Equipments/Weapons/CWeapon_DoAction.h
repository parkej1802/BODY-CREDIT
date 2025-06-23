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
		const TArray<FWeaponHitData>& InHitDatas,
		const TArray<FWeaponDoActionData>& InSprintDoActionDatas,
		const TArray<FWeaponHitData>& InSprintHitDatas
	);
	virtual void Tick(float InDeltaTime) {}

public:
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();

	virtual void SprintDoAction();
	
	/** Bow/Rifle 등 누름/뗌 기반 무기를 위한 가상 함수 */
	virtual void Pressed() {};   // 좌클릭 누름: 차징 시작
	virtual void Released() {};  // 좌클릭 뗌: 발사

public:
	UFUNCTION()
	virtual void OnBeginEquip() {}

	UFUNCTION()
	virtual void OnUnequip() {}

public:
	UFUNCTION()
	virtual void OnWeaponAttachmentBeginCollision() {}
				   
	UFUNCTION()
	virtual void OnWeaponAttachmentEndCollision() {};
	
	UFUNCTION()
	virtual void OnWeaponAttachmentBeginOverlap(class ACNox* InAttacker, AActor* InAttackCauser, class ACNox* InOther, const struct FHitResult& HitResult) {}
	
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
	
	TArray<FWeaponDoActionData> SprintDoActionDatas;
	TArray<FWeaponHitData> SprintHitDatas;

};
