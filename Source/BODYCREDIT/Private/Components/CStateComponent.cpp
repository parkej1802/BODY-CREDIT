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
	CheckTrue(IsAvoidMode());
	CheckTrue(IsActionMode());
	CheckTrue(IsDeadMode());
	
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
	// [중복 상태 변화 방지]
	// 이미 현재 상태(Type)와 바꾸려는 상태가 같으면 아무 동작도 하지 않음
	CheckTrue(Type == InType);

	// [상태 백업 및 변경]
	// 이전 상태를 저장한 뒤, 새로운 상태로 변경
	EStateType prevType = Type;
	Type = InType;

	// [델리게이트 호출]
	// 바인딩된 함수가 있으면 상태 변화 이벤트 Broadcast
	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, InType);
	
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