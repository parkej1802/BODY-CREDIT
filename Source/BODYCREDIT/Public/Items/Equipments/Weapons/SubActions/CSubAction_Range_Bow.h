#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/SubActions/CSubAction_Range.h"
#include "CSubAction_Range_Bow.generated.h"

UCLASS(Blueprintable)
class BODYCREDIT_API UCSubAction_Range_Bow : public UCSubAction_Range
{
	GENERATED_BODY()

public:
	UCSubAction_Range_Bow();

public:
	virtual void BeginPlay(class ACNox* InOwner, class ACWeapon_Attachment* InAttachment, class UCWeapon_DoAction* InDoAction) override;
	virtual void Tick_Implementation(float InDeltaTime) override;

public:
	virtual void Pressed() override;
	virtual void Released() override;
	
protected:
	virtual void OnAiming(FVector Output) override;

private:
	class ACAttachment_Bow* Bow;

private:
	float* Bend;
	
};
