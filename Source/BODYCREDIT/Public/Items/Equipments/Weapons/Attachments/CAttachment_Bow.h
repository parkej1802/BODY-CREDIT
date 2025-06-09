#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/CWeapon_Attachment.h"
#include "CAttachment_Bow.generated.h"

UCLASS()
class BODYCREDIT_API ACAttachment_Bow : public ACWeapon_Attachment
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "View")
	FVector2D ViewPitchRange = FVector2D(-45, +45);

public:
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(VisibleAnywhere)
	class UPoseableMeshComponent* PoseableMesh;

public:
	float* GetBend();

public:
	ACAttachment_Bow();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void OnBeginEquip_Implementation() override;
	void OnUnequip_Implementation() override;

private:
	FVector2D OriginViewPitchRange;

};
