// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Enemy/CNoxEnemyHPComponent.h"
#include "Characters/Enemy/CNox_EBase.h"

void UCNoxEnemyHPComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCNoxEnemyHPComponent::SetEnemy(class ACNox_EBase* InNewOwner)
{
	OwnerEnemy = InNewOwner;
}

void UCNoxEnemyHPComponent::HealHP(float InHealAmount)
{
	Health = FMath::Min(MaxHealth, Health + InHealAmount);
}
