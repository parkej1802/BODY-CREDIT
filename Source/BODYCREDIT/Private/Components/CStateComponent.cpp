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
