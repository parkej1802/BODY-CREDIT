#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/CEquipment_Weapon.h"
#include "CWeapon_Attachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponAttachmentBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponAttachmentEndCollision);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FWeaponAttachmentBeginOverlap, class ACNox*, InAttacker, AActor*, InAttackCuaser, class ACNox*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponAttachmentEndOverlap, class ACNox*, InAttacker, class ACNox*, InOther);

UCLASS()
class BODYCREDIT_API ACWeapon_Attachment : public ACEquipment_Weapon
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* Mesh;

public:
	ACWeapon_Attachment();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginEquip();
	virtual void OnBeginEquip_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void OnUnequip();
	virtual void OnUnequip_Implementation() {}

public:
	void OnCollisions();
	void OffCollisions();
	
private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UFUNCTION(BlueprintCallable, Category = "Attach")
	void AttachTo(FName InSocketName);

	UFUNCTION(BlueprintCallable, Category = "Attach")
	void AttachToCollision(FName InCollisionName);

	virtual void OnPickUpItem(class ACNox* InNewOwnerCharacter) override;

public:
	FWeaponAttachmentBeginCollision OnWeaponAttachmentBeginCollision;
	FWeaponAttachmentEndCollision OnWeaponAttachmentEndCollision;

	FWeaponAttachmentBeginOverlap OnWeaponAttachmentBeginOverlap;
	FWeaponAttachmentEndOverlap OnWeaponAttachmentEndOverlap;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	TArray<class UShapeComponent*> Collisions;

};
