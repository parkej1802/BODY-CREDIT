#include "Items/Equipments/Weapons/SubActions/CSubAction_Range_Rifle.h"
#include "Global.h"
#include "Curves/curvevector.h"

UCSubAction_Range_Rifle::UCSubAction_Range_Rifle()
{
	CHelpers::GetAsset<UCurveVector>(&Curve, TEXT("/Script/Engine.CurveVector'/Game/Items/Equipments/Weapons/Bow/Curve_Aiming.Curve_Aiming'"));
	
}

void UCSubAction_Range_Rifle::BeginPlay(class ACNox* InOwner, class ACWeapon_Attachment* InAttachment,
	class UCWeapon_DoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);
	
}

void UCSubAction_Range_Rifle::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);
	
}

void UCSubAction_Range_Rifle::Pressed()
{
	Super::Pressed();
	
}

void UCSubAction_Range_Rifle::Released()
{
	Super::Released();
	
}
