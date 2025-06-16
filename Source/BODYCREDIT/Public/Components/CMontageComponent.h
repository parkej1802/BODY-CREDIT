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

	int32 GetDirectionIndex(const FVector2D& InputDir, const FRotator& ControlRotation) const;
	int32 Get4DirectionIndex(const FVector2D& InputDir, const FRotator& ControlRotation) const;

public:
	void PlayAvoidMode(EWeaponType InWeaponType, const FVector2D& InputDir, const FRotator& ControlRotation);
	void PlayHittedMode(const FVector2D& InputDir, const FRotator& ControlRotation);
	void PlayDeadMode(const FVector2D& InputDir, const FRotator& ControlRotation);
	
	void PlayAnimMontage(EStateType InType);

private:	
	FMontageData* Datas[(int32)EStateType::MAX];

	// 8방향 모션
	FMontageData* Avoid[(int32)EWeaponType::MAX][8] = { nullptr, };
	FMontageData* Hitted[8] = { nullptr, };
	FMontageData* Dead[8] = { nullptr, };

	// 8방향 -> 4방향 fallback 테이블
	int32 FallbackDirectionIndex[8] = {
		0, // Forward
		0, // ForwardRight -> Forward
		2, // Right
		2, // BackwardRight -> Right
		4, // Backward
		4, // BackwardLeft -> Backward
		6, // Left
		0, // ForwardLeft -> Forward
	};

};
