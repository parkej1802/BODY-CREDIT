#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/CWeapon_SubAction.h"
#include "Components/TimelineComponent.h"
#include "CSubAction_Range.generated.h"

USTRUCT()
struct FAimDatas
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TargetArmLength = 100;

	UPROPERTY(EditAnywhere)
	FVector SocketOffset = FVector(0, 30, 10);

	UPROPERTY(EditAnywhere)
	bool bEnableCameraLag;

	UPROPERTY(EditAnywhere)
	FVector CameraLocation;
};

UCLASS()
class BODYCREDIT_API UCSubAction_Range : public UCWeapon_SubAction
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Aiming")
	UCurveVector* Curve;

	UPROPERTY(EditAnywhere, Category = "Aiming")
	FAimDatas AimData;

	UPROPERTY(EditAnywhere, Category = "Aiming")
	float AimingSpeed = 200;

	UPROPERTY()
	class USpringArmComponent* SpringArm;

	UPROPERTY()
	class UCameraComponent* Camera;

protected:
	FTimeline Timeline;

public:
	virtual void BeginPlay(class ACNox* InOwner, class ACWeapon_Attachment* InAttachment, class UCWeapon_DoAction* InDoAction) override;
	virtual void Tick_Implementation(float InDeltaTime) override;

public:
	virtual void Pressed() override;
	virtual void Released() override;

protected:
	UFUNCTION()
	virtual void OnAiming(FVector Output);

protected:
	FAimDatas OriginData;
	
};
