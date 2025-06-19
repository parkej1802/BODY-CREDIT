#include "Components/CMontageComponent.h"
#include "Global.h"
#include "Characters/CNox.h"
#include "Characters/CNox_Runner.h"
#include "Items/Equipments/Weapons/CWeapon_SubAction.h"

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
						Avoid[WeaponIndex][DirIndex] = data;
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

int32 UCMontageComponent::GetDirectionIndex(const FVector2D& InputDir, const FRotator& ControlRotation) const
{
	FVector2D Dir = InputDir.GetSafeNormal();
	if (Dir.IsNearlyZero())
		return 0; // Forward

	// 입력 벡터의 각도(라디안)
	float InputAngle = FMath::Atan2(Dir.Y, Dir.X);
	float InputAngleDeg = FMath::RadiansToDegrees(InputAngle);

	// 컨트롤러의 Yaw 기준으로 입력을 "내가 바라보는 방향"에 맞춰서 보정
	float Yaw = ControlRotation.Yaw;
	float RelativeAngleDeg = InputAngleDeg - Yaw;

	// 0~360도로 정규화, +90도(언리얼 Forward) 보정
	RelativeAngleDeg = FMath::Fmod(RelativeAngleDeg + 360.f + 90.f, 360.f);

	if (RelativeAngleDeg >= 337.5f || RelativeAngleDeg < 22.5f)   return 0; // Forward
	if (RelativeAngleDeg >= 22.5f && RelativeAngleDeg < 67.5f)    return 1; // ForwardRight
	if (RelativeAngleDeg >= 67.5f && RelativeAngleDeg < 112.5f)   return 2; // Right
	if (RelativeAngleDeg >= 112.5f && RelativeAngleDeg < 157.5f)  return 3; // BackwardRight
	if (RelativeAngleDeg >= 157.5f && RelativeAngleDeg < 202.5f)  return 4; // Backward
	if (RelativeAngleDeg >= 202.5f && RelativeAngleDeg < 247.5f)  return 5; // BackwardLeft
	if (RelativeAngleDeg >= 247.5f && RelativeAngleDeg < 292.5f)  return 6; // Left
	if (RelativeAngleDeg >= 292.5f && RelativeAngleDeg < 337.5f)  return 7; // ForwardLeft

	return 4; // Fallback (Backward)
}

int32 UCMontageComponent::Get4DirectionIndex(const FVector2D& InputDir, const FRotator& ControlRotation) const
{
	FVector2D Dir = InputDir.GetSafeNormal();
	if (Dir.IsNearlyZero())
		return 0; // Forward

	float InputAngle = FMath::Atan2(Dir.Y, Dir.X);
	float InputAngleDeg = FMath::RadiansToDegrees(InputAngle);
	float Yaw = ControlRotation.Yaw;
	float RelativeAngleDeg = InputAngleDeg - Yaw;
	RelativeAngleDeg = FMath::Fmod(RelativeAngleDeg + 360.f + 90.f, 360.f);

	// 4방향 분기 (0: Forward, 2: Right, 4: Backward, 6: Left)
	if (RelativeAngleDeg >= 315.f || RelativeAngleDeg < 45.f)   return 0; // Forward
	if (RelativeAngleDeg >= 45.f && RelativeAngleDeg < 135.f)   return 2; // Right
	if (RelativeAngleDeg >= 135.f && RelativeAngleDeg < 225.f)  return 4; // Backward
	return 6; // Left
}

void UCMontageComponent::PlayAvoidMode(EWeaponType InWeaponType, const FVector2D& InputDir, const FRotator& ControlRotation)
{
	CheckNull(OwnerCharacter);

	int32 WeaponIndex = (int32)InWeaponType;
	int32 DirIndex = 0;

	if (InWeaponType == EWeaponType::BOW)
	{
		DirIndex = Get4DirectionIndex(InputDir, ControlRotation);
	}
	else
	{
		DirIndex = GetDirectionIndex(InputDir, ControlRotation);
	}
	FMontageData* data = Avoid[WeaponIndex][DirIndex];

	// 8방향 데이터 없으면 4방향 fallback
	if (data == nullptr || data->Montage == nullptr)
	{
		int32 FallbackIndex = FallbackDirectionIndex[DirIndex];
		data = Avoid[WeaponIndex][FallbackIndex];
		if (data == nullptr || data->Montage == nullptr)
		{
			GLog->Log(ELogVerbosity::Error, "No Avoid Montage for direction or fallback");
			return;
		}
	}

	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);

	if (ACNox_Runner* runner = Cast<ACNox_Runner>(OwnerCharacter))
		runner->Weapon->GetSubAction()->Released();
	
	// if (FMontageData* data = AvoidDatas[WeaponIndex][DirsIndex])
	// {
	// 	if (data == nullptr || data->Montage == nullptr)
	// 	{
	// 		GLog->Log(ELogVerbosity::Error, "None montage data for Avoid");
	// 		return;
	// 	}
	// 	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
	// }
	
	// int32 Index = GetDirectionIndex(InputDir);
	// FMontageData* data = Avoid[Index];
	// if (data == nullptr || data->Montage == nullptr)
	// {
	// 	GLog->Log(ELogVerbosity::Error, "None Avoid montage data");
	// 	return;
	// }
	// OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
}

void UCMontageComponent::PlayHittedMode(const FVector2D& InputDir, const FRotator& ControlRotation)
{
	int32 Index = GetDirectionIndex(InputDir, ControlRotation);

	FMontageData* data = Hitted[Index];
	if (data == nullptr || data->Montage == nullptr)
	{
		int32 FallbackIndex = FallbackDirectionIndex[Index];
		data = Hitted[FallbackIndex];
		if (data == nullptr || data->Montage == nullptr)
		{
			GLog->Log(ELogVerbosity::Error, "No Hitted Montage for direction or fallback");
			return;
		}
	}
	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
	
	// if (FMontageData* data = Hitted[Index])
	// {
	// 	if (data == nullptr || data->Montage == nullptr)
	// 	{
	// 		GLog->Log(ELogVerbosity::Error, "None Hitted montage data");
	// 		return;
	// 	}
	// 	
	// 	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
	// }
}

void UCMontageComponent::PlayDeadMode(const FVector2D& InputDir, const FRotator& ControlRotation)
{
	int32 Index = GetDirectionIndex(InputDir, ControlRotation);

	FMontageData* data = Dead[Index];
	if (data == nullptr || data->Montage == nullptr)
	{
		int32 FallbackIndex = FallbackDirectionIndex[Index];
		data = Dead[FallbackIndex];
		if (data == nullptr || data->Montage == nullptr)
		{
			GLog->Log(ELogVerbosity::Error, "No Dead Montage for direction or fallback");
			return;
		}
	}
	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
	
	// if (FMontageData* data = Dead[Index])
	// {
	// 	if (data == nullptr || data->Montage == nullptr)
	// 	{
	// 		GLog->Log(ELogVerbosity::Error, "None Dead montage data");
	// 		return;
	// 	}
	// 	OwnerCharacter->PlayAnimMontage(data->Montage, data->PlayRate);
	// }
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
