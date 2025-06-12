#include "Components/CMontageComponent.h"
#include "Global.h"
#include "Characters/CNox.h"

UCMontageComponent::UCMontageComponent()
{

}

void UCMontageComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DataTable == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "DataTable is not selected");

		return;
	}

	TArray<FMontageData*> datas;
	DataTable->GetAllRows<FMontageData>("", datas);

	for (int32 i = 0; i < (int32)EStateType::MAX; ++i)
	{
		for (FMontageData* data : datas)
		{
			if ((EStateType)i == data->Type)
			{
				Datas[i] = data;

				continue;
			}
		} //for(data)
	} //for(i)

}

void UCMontageComponent::PlayBackStepMode()
{
	PlayAnimMontage(EStateType::Avoid);

}

void UCMontageComponent::PlayDeadMode()
{
	PlayAnimMontage(EStateType::Dead);

}

void UCMontageComponent::PlayAnimMontage(EStateType InType)
{
	CheckNull(OwnerCharacter);

	FMontageData* data = Datas[(int32)InType];

	if (data == nullptr || data->Montage == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "None montages data");

		return;
	}

	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);

}
