#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/CWeapon_DoAction.h"
#include "CDoAction_Bow.generated.h"

UENUM()
enum class EArrowChargeStage : uint8
{
	Stage1, Stage2, Stage3, MAX
};

UCLASS(Blueprintable)
class BODYCREDIT_API UCDoAction_Bow : public UCWeapon_DoAction
{
	GENERATED_BODY()

// public:
// 	UCDoAction_Bow();
//
// 	virtual void BeginPlay
// 	(
// 		class ACWeapon_Attachment* InAttachment,
// 		class UCWeapon_Equipment* InEquipment,
// 		class ACNox* InOwner,
// 		const TArray<FWeaponDoActionData>& InDoActionDatas,
// 		const TArray<FWeaponHitData>& InHitDatas,
// 		const TArray<FWeaponDoActionData>& InSprintDoActionDatas,
// 		const TArray<FWeaponHitData>& InSprintHitDatas
// 	) override;
// 	virtual void Tick(float InDeltaTime) override;
//
// 	virtual void Pressed() override;
// 	virtual void Released() override;
//
// public:
// 	void End_BowString();
// 	
// private:
// 	void StartCharging();
// 	void UpdateChargeStage();
// 	void ReleaseCharging();
//
// 	void CreateArrow();
// 	class ACAddOn_Arrow* GetAttachedArrow();
//
// 	UFUNCTION()
// 	void OnArrowHit(class AActor* InCauser, class ACNox* InOtherCharacter);
//
// 	UFUNCTION()
// 	void OnArrowEndPlay(class ACAddOn_Arrow* InDestroyer);
//
// private:
// 	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
// 	TSubclassOf<class ACAddOn_Arrow> ArrowClass;
//
// 	class USkeletalMeshComponent* SkeletalMesh;
// 	class UPoseableMeshComponent* PoseableMesh;
// 	UPROPERTY()
// 	class ACAttachment_Bow* Bow;
//
// 	float* Bending;
// 	FVector OriginLocation;
// 	bool bAttachedString = true;
// 	const bool* bEquipped;
//
// 	bool bIsCharging = false;
// 	float ChargeTime = 0.0f;
// 	FTimerHandle ChargeTimerHandle;
// 	EArrowChargeStage ChargeStage = EArrowChargeStage::Stage1;

public:
	UCDoAction_Bow();

	virtual void BeginPlay
	(
		class ACWeapon_Attachment* InAttachment,
		class UCWeapon_Equipment* InEquipment,
		class ACNox* InOwner,
		const TArray<FWeaponDoActionData>& InDoActionDatas,
		const TArray<FWeaponHitData>& InHitDatas,
		const TArray<FWeaponDoActionData>& InSprintDoActionDatas,
		const TArray<FWeaponHitData>& InSprintHitDatas
	) override;

	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

	void OnBeginEquip() override;
	void OnUnequip() override;

	void Tick(float InDeltaTime) override;

public:
	void End_BowString();

private:
	class ACAddOn_Arrow* GetAttachedArrow();

private:
	UFUNCTION()
	void OnArrowHit(class AActor* InCauser, class ACNox* InOtherCharacter);

	UFUNCTION()
	void OnArrowEndPlay(class ACAddOn_Arrow* InDestroyer);

private:
	class USkeletalMeshComponent* SkeletalMesh;
	class UPoseableMeshComponent* PoseableMesh;

private:
	UPROPERTY()
	class ACAttachment_Bow* Bow;
	
private:
	float* Bending;

private:
	FVector OriginLocation;
	bool bAttachedString = true;

private:
	const bool* bEquipped;

	bool bCanShoot = true;
	
};
