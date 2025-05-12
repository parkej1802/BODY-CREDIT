#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeapon_SubAction.generated.h"

UCLASS()
class BODYCREDIT_API UCWeapon_SubAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetInAction() { return bInAction; }

public:
	UCWeapon_SubAction();

public:
	virtual void BeginPlay(class ACNox* InOwner, class ACWeapon_Attachment* InAttachment, class UCWeapon_DoAction* InDoAction);

public:
	virtual void Pressed();
	virtual void Released();

public:
	UFUNCTION(BlueprintNativeEvent)
	void Begin_SubAction();
	virtual void Begin_SubAction_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void End_SubAction();
	virtual void End_SubAction_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void Tick(float InDeltaTime);
	virtual void Tick_Implementation(float InDeltaTime) {}

protected:
	bool bInAction;

	class ACNox* Owner;
	class ACWeapon_Attachment* Attachment;
	class UCWeapon_DoAction* DoAction;

	class UCStateComponent* State;
	class UCMovementComponent* Movement;
	
};
