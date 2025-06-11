#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/CWeapon_DoAction.h"
#include "CDoAction_Bow.generated.h"

UCLASS(Blueprintable)
class BODYCREDIT_API UCDoAction_Bow : public UCWeapon_DoAction
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
	TSubclassOf<class ACAddOn_Arrow> ArrowClass;

public:
	UCDoAction_Bow();

	void BeginPlay
	(
		class ACWeapon_Attachment* InWeaponAttachment,
		class UCWeapon_Equipment* InEquipment,
		class ACNox* InOwner,
		const TArray<FWeaponDoActionData>& InDoActionData,
		const TArray<FWeaponHitData>& InHitData
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
	void CreateArrow();
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
	float* Bending;

private:
	FVector OriginLocation;
	bool bAttachedString = true;

private:
	const bool* bEquipped;

private:
	UPROPERTY()
	class ACAttachment_Bow* Bow;

};
