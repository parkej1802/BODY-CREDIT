#include "Components/CNoxHPComponent.h"

UCNoxHPComponent::UCNoxHPComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCNoxHPComponent::BeginPlay()
{
	Super::BeginPlay();
	InitStatus();
}

void UCNoxHPComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCNoxHPComponent::InitStatus()
{
	Health = MaxHealth;
	Defense = MaxDefense;
}

void UCNoxHPComponent::TakeDamage(float Amount)
{
}

void UCNoxHPComponent::Die()
{
}
