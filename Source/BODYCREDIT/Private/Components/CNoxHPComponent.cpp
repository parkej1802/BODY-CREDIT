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

void UCNoxHPComponent::TakeDamage(float Amount, bool ActiveShield, bool& OutIsShieldCrash)
{
	if (ActiveShield)
	{
		Defense -= Amount;
		if (Defense <= 0)
		{
			OutIsShieldCrash = true;
			Health += Defense; // 초과된 데미지 만큼 HP 차감
			Defense = 0;
		}
	}
	else
	{
		Health = FMath::Max(0.f, Health - Amount);		
	}
}

void UCNoxHPComponent::Die()
{
}
