#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/SubActions/CSubAction_Range.h"
#include "CSubAction_Range_Rifle.generated.h"

UCLASS()
class BODYCREDIT_API UCSubAction_Range_Rifle : public UCSubAction_Range
{
	GENERATED_BODY()

public:
	UCSubAction_Range_Rifle();

protected:
	virtual void BeginPlay(class ACNox* InOwner, class ACWeapon_Attachment* InAttachment, class UCWeapon_DoAction* InDoAction) override;
	virtual void Tick_Implementation(float InDeltaTime) override;
	
protected:
	virtual void Pressed() override;
	virtual void Released() override;
	
};
