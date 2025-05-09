// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/Enemy/CNoxEnemyHPComponent.h"
#include "Characters/Enemy/CNox_EBase.h"

void UCNoxEnemyHPComponent::SetEnemy(class ACNox_EBase* InNewOwner)
{
	OwnerEnemy = InNewOwner;
}
