#include "Components/Enemy/CNox_BehaviorComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

UCNox_BehaviorComponent::UCNox_BehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCNox_BehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCNox_BehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCNox_BehaviorComponent::SetEnemyType(EEnemyType InType)
{
	Blackboard->SetValueAsEnum(AIStateTypeKey, (uint8)InType);
}

