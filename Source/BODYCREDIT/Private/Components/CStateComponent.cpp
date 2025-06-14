#include "Components/CStateComponent.h"
#include "Global.h"

UCStateComponent::UCStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UCStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);

}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);

}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);

}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);

}

void UCStateComponent::SetAvoidMode()
{
	ChangeType(EStateType::Avoid);

}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);

}

void UCStateComponent::ChangeType(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, Type);

}

void UCStateComponent::OnSubActionMode()
{
	bInSubActionMode = true;

}

void UCStateComponent::OffSubActionMode()
{
	bInSubActionMode = false;

}

EAvoidDirection8Way UCStateComponent::GetAvoidDirection(const FVector2D& InputVector, const FRotator& ControlRotation) const
{
	if (InputVector.SizeSquared() < KINDA_SMALL_NUMBER)
	{
		return EAvoidDirection8Way::Forward;
	}

	FVector WorldDir = FRotationMatrix(ControlRotation).TransformVector(FVector(InputVector.Y, InputVector.X, 0.f));
	WorldDir.Z = 0;
	WorldDir.Normalize();

	float Angle = FMath::Atan2(WorldDir.Y, WorldDir.X);
	float Degree = FMath::Fmod(FMath::RadiansToDegrees(Angle) + 360.f, 360.f);

	if (Degree < 22.5f || Degree >= 337.5f) return EAvoidDirection8Way::Forward;
	if (Degree < 67.5f) return EAvoidDirection8Way::ForwardRight;
	if (Degree < 112.5f) return EAvoidDirection8Way::Right;
	if (Degree < 157.5f) return EAvoidDirection8Way::BackwardRight;
	if (Degree < 202.5f) return EAvoidDirection8Way::Backward;
	if (Degree < 247.5f) return EAvoidDirection8Way::BackwardLeft;
	if (Degree < 292.5f) return EAvoidDirection8Way::Left;
	return EAvoidDirection8Way::ForwardLeft;
}

void UCStateComponent::SetLastAvoidDirection(EAvoidDirection8Way InDirection)
{
	LastAvoidDirection = InDirection;
}

EAvoidDirection8Way UCStateComponent::GetLastAvoidDirection() const
{
	return LastAvoidDirection;
}