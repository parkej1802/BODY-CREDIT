#include "Items/Equipments/Weapons/SubActions/CSubAction_Range_Bow.h"
#include "Global.h"
#include "Curves/curvevector.h"
#include "Items/Equipments/Weapons/Attachments/CAttachment_Bow.h"
#include "Items/Equipments/Weapons/AddOns/CAddOn_Arrow.h"
#include "gameframework/projectilemovementcomponent.h"

UCSubAction_Range_Bow::UCSubAction_Range_Bow()
{
	CHelpers::GetAsset<UCurveVector>(&Curve, TEXT("/Script/Engine.CurveVector'/Game/Items/Equipments/Weapons/Bow/Curve_Aiming.Curve_Aiming'"));
}

void UCSubAction_Range_Bow::BeginPlay(ACNox* InOwner, ACWeapon_Attachment* InAttachment, UCWeapon_DoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	Bow = Cast<ACAttachment_Bow>(InAttachment);
	if (!!Bow)
	{
		Bend = Bow->GetBend();
	}
}

void UCSubAction_Range_Bow::Tick_Implementation(float InDeltaTime)
{
	Super::Tick_Implementation(InDeltaTime);

	if (bInAction)
	{
		CheckNull(Bow);
		CheckTrue(Bow->Arrows.IsEmpty());
		Bow->Arrows.Last()->SetActorHiddenInGame(false);
	}
	else
	{
		CheckNull(Bow);
		CheckTrue(Bow->Arrows.IsEmpty());
		CheckTrue(Bow->Arrows.Last()->GetProjectileMovement()->IsActive());
		Bow->Arrows.Last()->SetActorHiddenInGame(true);
	}
	
}

void UCSubAction_Range_Bow::OnAiming(FVector Output)
{
	Super::OnAiming(Output);

	if (Bend)
	{
		*Bend = Output.Y;
	}
}

void UCSubAction_Range_Bow::Pressed()
{
	Super::Pressed();
	
	Timeline.PlayFromStart();
}

void UCSubAction_Range_Bow::Released()
{
	Super::Released();
	
	Timeline.Reverse();
}
