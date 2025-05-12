#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Engine/DamageEvents.h"
#include "CWeapon_Structures.generated.h"

USTRUCT(BlueprintType) // 장착
struct FWeaponEquipmentData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere)
	bool bUseControlRotation = true;

};

USTRUCT(BlueprintType) // 공격
struct FWeaponDoActionData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere)
	bool bFixedCamera;

	UPROPERTY(EditAnywhere)
	class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

public:
	void DoAction(class ACNox* InOwner);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);

};

USTRUCT(BlueprintType) // 피격
struct FWeaponHitData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;

	UPROPERTY(EditAnywhere)
	float Power;

	UPROPERTY(EditAnywhere)
	float Launch = 100;

	UPROPERTY(EditAnywhere)
	float StopTime;

	UPROPERTY(EditAnywhere)
	class USoundWave* Sound;

	UPROPERTY(EditAnywhere)
	class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

public:
	void SendDamage(class ACNox* InAttacker, AActor* InAttackCauser, class ACNox* InOther);
	void PlayMontage(class ACNox* InOwner);
	void PlayHitStop(UWorld* InWorld);
	void PlaySoundWave(class ACNox* InOwner);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation);

};

USTRUCT()
struct FActionDamageEvent
	: public FDamageEvent
{
	GENERATED_BODY()

public:
	FWeaponHitData* WeaponHitData;

};

UCLASS()
class BODYCREDIT_API UCWeapon_Structures : public UObject
{
	GENERATED_BODY()
	
};
