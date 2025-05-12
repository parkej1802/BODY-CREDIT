#pragma once

#include "CoreMinimal.h"
#include "Items/Equipments/Weapons/CWeapon_Attachment.h"
#include "CAttachment_Sniper.generated.h"

UCLASS()
class BODYCREDIT_API ACAttachment_Sniper : public ACWeapon_Attachment
{
	GENERATED_BODY()

public:
	ACAttachment_Sniper();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
};
