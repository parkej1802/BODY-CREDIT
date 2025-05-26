#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "Interfaces/IBindInput.h"
#include "CWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	REVOLVER, RIFLE, SNIPER, MAX,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS()
class BODYCREDIT_API UCWeaponComponent
	: public UCBaseComponent
	, public IIBindInput
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Enhanced")
	class UInputAction* IA_FirstWeapon;

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
	FORCEINLINE bool IsRevovlerMode() { return Type == EWeaponType::REVOLVER; }
	FORCEINLINE bool IsRifleMode() { return Type == EWeaponType::RIFLE; }
	FORCEINLINE bool IsSniperMode() { return Type == EWeaponType::SNIPER; }
	FORCEINLINE bool IsUnarmedMode() { return Type == EWeaponType::MAX; }

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

public:
	UFUNCTION(BlueprintCallable)
	class UCWeapon_SubAction* GetSubAction();

public:
	void SetRevolverMode();
	void SetRifleMode();
	void SetSniperMode();
	void SetUnarmedMode();

	void DoAction();

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

};
