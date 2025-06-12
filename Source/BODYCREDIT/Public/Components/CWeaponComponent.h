#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "Interfaces/IBindInput.h"
#include "CWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	Weapon1, Weapon2, MAX,
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	KATANA, BOW, RIFLE, MAX,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS()
class BODYCREDIT_API UCWeaponComponent
	: public UCBaseComponent
	, public IIBindInput
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UEnhancedInputComponent* EnhancedInputComponent;

private:
	// UPROPERTY(VisibleAnywhere, Category = "Enhanced")
	// class UInputAction* IA_Bow;
	//
	// UPROPERTY(VisibleAnywhere, Category = "Enhanced")
	// class UInputAction* IA_Rifle;
	//
	// UPROPERTY(VisibleAnywhere, Category = "Enhanced")
	// class UInputAction* IA_Katana;

	UPROPERTY(VisibleAnywhere, Category = "Enhanced")
	class UInputAction* IA_WeaponSlot1;

	UPROPERTY(VisibleAnywhere, Category = "Enhanced")
	class UInputAction* IA_WeaponSlot2;

	UPROPERTY(VisibleAnywhere, Category = "Enhanced")
	class UInputAction* IA_Action;

	UPROPERTY(VisibleAnywhere, Category = "Enhanced")
	class UInputAction* IA_SubAction;

private:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
	class UCWeapon_Asset* DataAssets[(int32)EWeaponType::MAX];

public:
	FORCEINLINE EWeaponType GetWeaponType() { return Type; }
 
public:
	FORCEINLINE bool IsKatanaMode() { return Type == EWeaponType::KATANA; }
	FORCEINLINE bool IsBowMode() { return Type == EWeaponType::BOW; }
	FORCEINLINE bool IsRifleMode() { return Type == EWeaponType::RIFLE; }
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::MAX;}

public:
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void BindInput(UEnhancedInputComponent* InEnhancedInputComponent) override;

private:
	bool IsIdleMode();

public:
	class ACWeapon_Attachment* GetAttachment();
	class UCWeapon_Equipment* GetEquipment();
	class UCWeapon_DoAction* GetDoAction();

	// class UInputAction* GetBow() { return IA_Bow; }
	// class UInputAction* GetKatana() { return IA_Katana; }
	// class UInputAction* GetRifle() { return IA_Rifle; }
	
public:
	UFUNCTION(BlueprintCallable)
	class UCWeapon_SubAction* GetSubAction();

public:	
	void SetKatanaMode();
	void SetBowMode();
	void SetRifleMode();
	void SetUnarmedMode();

	void SetWeaponSlot1();
	void SetWeaponSlot2();
	void DoAction();
	void EndDoAction();

public:
	UFUNCTION(BlueprintCallable)
	void SubAction_Pressed();

	UFUNCTION(BlueprintCallable)
	void SubAction_Released();

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);

public:
	FWeaponTypeChanged OnWeaponTypeChange;

private:
	EWeaponType Type = EWeaponType::MAX;

private:
	UPROPERTY()
	class UCWeapon_Data* Datas[(int32)EWeaponType::MAX];
	
private:
	bool bInSubAction = false;

};
