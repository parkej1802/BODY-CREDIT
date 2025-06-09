#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
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
	EStateType Type;

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
	void PlayAnimMontage(EStateType InType);

private:
	FMontageData* Datas[(int32)EStateType::MAX];

};
