#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/DoActions/CWeapon_DoAction_Combo.h"
#include "CDoAction_Combo_Bow.generated.h"

UCLASS(Blueprintable)
class BODYCREDIT_API UCDoAction_Combo_Bow : public UCWeapon_DoAction_Combo
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
	TSubclassOf<class ACAddOn_Arrow> ArrowClass;

public:
	UCDoAction_Combo_Bow();
	
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

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	void OnBeginEquip() override;
	void OnUnequip() override;

	void Tick(float InDeltaTime) override;

public:
	void End_BowString();

private:
	void CreateArrow();
	class ACAddOn_Arrow* GetAttachedArrow();

private:
	UFUNCTION()
	void OnArrowHit(class AActor* InCauser, class ACNox* InOtherCharacter, const struct FHitResult& HitResult);

	UFUNCTION()
	void OnArrowEndPlay(class ACAddOn_Arrow* InDestroyer);

private:
	class USkeletalMeshComponent* SkeletalMesh;
	class UPoseableMeshComponent* PoseableMesh;

private:
	float* Bending;

private:
	FVector OriginLocation;
	bool bAttachedString = true;

private:
	const bool* bEquipped;

private:
	TArray<class ACAddOn_Arrow*> Arrows;

	bool bInAction = false;
	
};
