#pragma once

#include "CoreMinimal.h"
#include "Characters/CNox.h"
#include "CNox_Player.generated.h"

UCLASS()
class BODYCREDIT_API ACNox_Player : public ACNox
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector2D PitchRange = FVector2D(-40, +40);

protected: // Modular Character Mesh
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Hair;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* UpperBody;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Arms;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Backpack;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* ChestRig;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* LowerBody;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Foot;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Weapon1;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mesh")
	class USkeletalMeshComponent* Weapon2;
	
public:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* FPSArms;

public:
	UPROPERTY(VisibleAnywhere)
	class UCGunComponent* Weapon;

public:
	FORCEINLINE USkeletalMeshComponent* GetArms() { return Arms; }

public:
	ACNox_Player();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMoveForward(float InAxisValue);
	void OnMoveRight(float InAxisValue);
	void OnHorizontalLook(float InAxisValue);
	void OnVerticalLook(float InAxisValue);

private:
	void OnRun();
	void OffRun();
	
};
