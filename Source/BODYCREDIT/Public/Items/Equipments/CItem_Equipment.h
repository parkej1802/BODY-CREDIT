#pragma once

#include "CoreMinimal.h"
#include "Items/CItem.h"
#include "CItem_Equipment.generated.h"

UCLASS()
class BODYCREDIT_API ACItem_Equipment : public ACItem
{
	GENERATED_BODY()
	
public:
	ACItem_Equipment();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
