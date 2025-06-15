#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Engine/DataTable.h"
#include "CMontageComponent.generated.h"

USTRUCT()
struct FMontageData
	: public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;
	
	UPROPERTY(EditAnywhere)
	EStateType StateType;

	UPROPERTY(EditAnywhere)
	FName Direction; 

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;
};

UCLASS()
class BODYCREDIT_API UCMontageComponent : public UCBaseComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "DataTable")
	UDataTable* DataTable;

public:
	UCMontageComponent();

protected:
	virtual void BeginPlay() override;

public:
	void PlayBackStepMode();
	void PlayDeadMode();

private:
	int32 GetDirectionIndexByTag(const FName& DirectionTag) const;

	int32 GetDirectionIndex(const FVector2D& InputDir) const;

public:
	void PlayAvoidMode(EWeaponType InWeaponType, const FVector2D& InputDir);
	void PlayHittedMode(const FVector2D& InputDir);
	void PlayDeadMode(const FVector2D& InputDir);
	
	void PlayAnimMontage(EStateType InType);

private:	
	FMontageData* Datas[(int32)EStateType::MAX];

	// 8방향 모션
	FMontageData* AvoidDatas[(int32)EWeaponType::MAX][8] = { nullptr, };
	FMontageData* Hitted[8] = { nullptr, };
	FMontageData* Dead[8] = { nullptr, };

};
