#pragma once

#include "CoreMinimal.h"
#include "Components/CBaseComponent.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
	Idle = 0, Equip, Action, Hitted, Avoid, Dead, MAX,
};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UENUM(BlueprintType)
enum class EAvoidDirection8Way : uint8
{
	Forward,
	ForwardRight,
	Right,
	BackwardRight,
	Backward,
	BackwardLeft,
	Left,
	ForwardLeft,
	MAX
};

UCLASS()
class BODYCREDIT_API UCStateComponent : public UCBaseComponent
{
	GENERATED_BODY()
	
public:
	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsAvoidMode() { return Type == EStateType::Avoid; }
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }

	FORCEINLINE bool IsSubActionMode() { return bInSubActionMode; }

public:
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetIdleMode();
	void SetEquipMode();
	void SetActionMode();
	void SetHittedMode();
	void SetAvoidMode();
	void SetDeadMode();

	void OnSubActionMode();
	void OffSubActionMode();

private:
	void ChangeType(EStateType InType);

public:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;

private:
	bool bInSubActionMode;

public:
	EAvoidDirection8Way GetAvoidDirection(const FVector2D& InputVector, const FRotator& ControlRotation) const;

	void SetLastAvoidDirection(EAvoidDirection8Way InDirection);
	EAvoidDirection8Way GetLastAvoidDirection() const;

private:
	EAvoidDirection8Way LastAvoidDirection;

};
