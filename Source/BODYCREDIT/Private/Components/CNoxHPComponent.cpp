#include "Components/CNoxHPComponent.h"
#include "Global.h"
#include "Characters/CNox_Runner.h"
#include "Components/CStateComponent.h"

UCNoxHPComponent::UCNoxHPComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCNoxHPComponent::BeginPlay()
{
	Super::BeginPlay();
	InitStatus();

	OwnerCharacter = Cast<ACNox_Runner>(GetOwner());

}

void UCNoxHPComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCNoxHPComponent::InitStatus()
{
	Health = MaxHealth;
	Stamina = MaxStamina;
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

void UCNoxHPComponent::TakeDamage(float Amount)
{
	CheckNull(OwnerCharacter);
	CheckTrue(CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsAvoidMode());
	
	Health = FMath::Max(0.f, Health - Amount);
	SetHealth(Health);
	if (Health <= 0) Die();
	// CLog::Print(FString::Printf(TEXT("%s TakeDamage: %f"), *GetOwner()->GetName() ,Amount));
}

void UCNoxHPComponent::Die()
{
	bIsDead = true;

	CheckNull(OwnerCharacter);
	if (UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter))
	{
		state->SetDeadMode();
	}

}

void UCNoxHPComponent::SetStatus(float newHP, float newDefense)
{
	Health = newHP;
	Defense = newDefense;
}

void UCNoxHPComponent::SetHealth(float InNewHealth)
{
	Health = FMath::Clamp(InNewHealth, 0.f, MaxHealth);
	OnHealthChanged.Broadcast(Health, MaxHealth);

}

void UCNoxHPComponent::SetStamina(float InNewStamina)
{
	Stamina = FMath::Clamp(InNewStamina, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(Stamina, MaxStamina);

}