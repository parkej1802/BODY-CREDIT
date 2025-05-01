#include "Components/CBaseComponent.h"
#include "Global.h"
#include "Characters/CNox.h"

UCBaseComponent::UCBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UCBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACNox>(GetOwner());

}

void UCBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
