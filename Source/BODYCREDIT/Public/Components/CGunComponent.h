#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGunComponent.generated.h"

UENUM(BlueprintType)
enum class EGunType : uint8
{
	AR, AK47, Pistol, Max,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGunTypeChanged, EGunType, InPrevType, EGunType, InNewType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponAim_Arms_Begin, class ACWeapon*, InThisWeapon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeaponAim_Arms_End);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYCREDIT_API UCGunComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	TArray<TSubclassOf<class ACWeapon>> WeaponClasses;

	// UPROPERTY(EditAnywhere, Category = "Settings")
	// TSubclassOf<class UCUserWidget_HUD> HUDClass;

public:
	FORCEINLINE bool IsUnarmedMode() { return Type == EGunType::Max; }
	FORCEINLINE bool IsAR4Mode() { return Type == EGunType::AR; }
	FORCEINLINE bool IsAK47Mode() { return Type == EGunType::AK47; }
	FORCEINLINE bool IsPistolMode() { return Type == EGunType::Pistol; }

public:	
	UCGunComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		

private:
	class ACWeapon* GetCurrWeapon();

public:
	void SetUnarmedMode();
	void SetARMode();
	void SetAK47Mode();
	void SetPistolMode();

private:
	void SetMode(EGunType InType);
	void ChangeType(EGunType InType);

public:
	void Begin_Equip();
	void End_Equip();

public:
	void Begin_Aim();
	void End_Aim();

public:
	void Begin_Fire();
	void End_Fire();

public:
	bool IsInAim();
	FVector GetLeftHandLocation();
	FTransform GetLeftHandTransform();

public:
	void ToggleAutoFire();

public:
	void Reload();
	void Eject_Magazine();
	void Spawn_Magazine();
	void Load_Magazine();
	void End_Reload();

private:
	UFUNCTION()
	void On_Begin_Aim(class ACWeapon* InThisWeapon);

	UFUNCTION()
	void On_Begin_End();

public:
	FGunTypeChanged OnWeaponTypeChanged;
	FWeaponAim_Arms_Begin OnWeaponAim_Arms_Begin;
	FWeaponAim_Arms_End OnWeaponAim_Arms_End;

private:
	EGunType Type = EGunType::Max;

private:
	class ACNox_Runner* Owner;
	TArray<class ACWeapon*> Weapons;
	// class UCUserWidget_HUD* HUD;
};
