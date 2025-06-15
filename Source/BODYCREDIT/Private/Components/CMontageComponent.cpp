#include "Components/CMontageComponent.h"
#include "Global.h"
#include "Characters/CNox.h"

UCMontageComponent::UCMontageComponent()
{
	CHelpers::GetAsset<UDataTable>(&DataTable, TEXT("/Script/Engine.DataTable'/Game/Characters/Runner/DataTables/DT_Montage.DT_Montage'"));
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

	for (FMontageData* data : datas)
	{
		int32 Index = GetDirectionIndexByTag(data->Direction);

		switch (data->StateType)
		{
			case EStateType::Avoid:
				{
					int32 WeaponIndex = (int32)data->WeaponType;
					int32 DirIndex = GetDirectionIndexByTag(data->Direction);
					if (WeaponIndex >= 0 && WeaponIndex < (int32)EWeaponType::MAX
						&& DirIndex >= 0 && DirIndex < 8)
						AvoidDatas[WeaponIndex][DirIndex] = data;
				}
				break;
			
			case EStateType::Hitted:
				{
					if (Index >= 0 && Index < 8)
						Hitted[Index] = data;
				}
				break;
			
			case EStateType::Dead:
				{
					if (Index >= 0 && Index < 8)
						Dead[Index] = data;
				}
				break;
		}
		
	// 	if (data->Type == EStateType::Avoid)
	// 	{
	// 		int32 WeaponIndex = (int32)data->WeaponType;
	// 		int32 DirIndex = GetDirectionIndexByTag(data->Direction);
	// 		if (WeaponIndex >= 0 && WeaponIndex < (int32)EWeaponType::MAX
	// 			&& DirIndex >= 0 && DirIndex < 8)
	// 		{
	// 			AvoidDatas[WeaponIndex][DirIndex] = data;
	// 		}
	// 	}
	// 	else
	// 	{
	// 		
	// 	}
	// 	// ... (Hitted, Dead 등 기존 분기 유지)
	// }
	//
	// TArray<FMontageData*> datas;
	// DataTable->GetAllRows<FMontageData>("", datas);
	//
	// for (FMontageData* data : datas)
	// {
	// 	int32 Index = GetDirectionIndexByTag(data->Direction); // 아래 참고
	//
	// 	switch (data->Type)
	// 	{
	// 	case EStateType::Avoid:
	// 		if (Index >= 0 && Index < 8)
	// 			Avoid[Index] = data;
	// 		break;
	// 	case EStateType::Hitted:
	// 		if (Index >= 0 && Index < 8)
	// 			Hitted[Index] = data;
	// 		break;
	// 	case EStateType::Dead:
	// 		if (Index >= 0 && Index < 8)
	// 			Dead[Index] = data;
	// 		break;
	// 	default:
	// 		break;
	// 	}
	} // for data
	
	// TArray<FMontageData*> datas;
	// DataTable->GetAllRows<FMontageData>("", datas);
	//
	// for (int32 i = 0; i < (int32)EStateType::MAX; ++i)
	// {
	// 	for (FMontageData* data : datas)
	// 	{
	// 		if ((EStateType)i == data->Type)
	// 		{
	// 			Datas[i] = data;
	//
	// 			continue;
	// 		}
	// 	} //for(data)
	// } //for(i)

}

void UCMontageComponent::PlayBackStepMode()
{
	PlayAnimMontage(EStateType::Avoid);

}

void UCMontageComponent::PlayDeadMode()
{
	PlayAnimMontage(EStateType::Dead);

}

int32 UCMontageComponent::GetDirectionIndexByTag(const FName& InDirection) const
{
	if (InDirection == "Forward")         return 0;
	if (InDirection == "ForwardRight")    return 1;
	if (InDirection == "Right")           return 2;
	if (InDirection == "BackwardRight")   return 3;
	if (InDirection == "Backward")        return 4;
	if (InDirection == "BackwardLeft")    return 5;
	if (InDirection == "Left")            return 6;
	if (InDirection == "ForwardLeft")     return 7;
	return 0;
}

int32 UCMontageComponent::GetDirectionIndex(const FVector2D& InputDir) const
{
	FVector2D Dir = InputDir.GetSafeNormal();
	if (Dir.IsNearlyZero())
		return 0; // Default Forward

	// atan2(Y, X) 기준, 언리얼은 Forward가 X+, Right가 Y+
	float AngleRad = FMath::Atan2(Dir.Y, Dir.X);
	float AngleDeg = FMath::RadiansToDegrees(AngleRad);

	// 언리얼 Forward 기준이 0도(오른쪽이 90도)로 맞춰주기 위해 +90
	AngleDeg = FMath::Fmod(AngleDeg + 360.f + 90.f, 360.f);

	// 45도 단위로 8분할, 각 방향의 중앙값 기준
	if (AngleDeg >= 337.5f || AngleDeg < 22.5f)
		return 0; // Forward
	if (AngleDeg >= 22.5f && AngleDeg < 67.5f)
		return 1; // ForwardRight
	if (AngleDeg >= 67.5f && AngleDeg < 112.5f)
		return 2; // Right
	if (AngleDeg >= 112.5f && AngleDeg < 157.5f)
		return 3; // BackwardRight
	if (AngleDeg >= 157.5f && AngleDeg < 202.5f)
		return 4; // Backward
	if (AngleDeg >= 202.5f && AngleDeg < 247.5f)
		return 5; // BackwardLeft
	if (AngleDeg >= 247.5f && AngleDeg < 292.5f)
		return 6; // Left
	if (AngleDeg >= 292.5f && AngleDeg < 337.5f)
		return 7; // ForwardLeft

	return 4; // Fallback (Backward)
}

void UCMontageComponent::PlayAvoidMode(EWeaponType InWeaponType, const FVector2D& InputDir)
{
	int32 WeaponIndex = (int32)InWeaponType;
	int32 DirIndex = GetDirectionIndex(InputDir);

	FMontageData* data = AvoidDatas[WeaponIndex][DirIndex];

	if (data == nullptr || data->Montage == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "None montage data for Avoid");
		return;
	}
	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
	
	// int32 Index = GetDirectionIndex(InputDir);
	// FMontageData* data = Avoid[Index];
	// if (data == nullptr || data->Montage == nullptr)
	// {
	// 	GLog->Log(ELogVerbosity::Error, "None Avoid montage data");
	// 	return;
	// }
	// OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
}

void UCMontageComponent::PlayHittedMode(const FVector2D& InputDir)
{
	int32 Index = GetDirectionIndex(InputDir);
	FMontageData* data = Hitted[Index];
	if (data == nullptr || data->Montage == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "None Hitted montage data");
		return;
	}
	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
}

void UCMontageComponent::PlayDeadMode(const FVector2D& InputDir)
{
	int32 Index = GetDirectionIndex(InputDir);
	FMontageData* data = Dead[Index];
	if (data == nullptr || data->Montage == nullptr)
	{
		GLog->Log(ELogVerbosity::Error, "None Dead montage data");
		return;
	}
	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
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
