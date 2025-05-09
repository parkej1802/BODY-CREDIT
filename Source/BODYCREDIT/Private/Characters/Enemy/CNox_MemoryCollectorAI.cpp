// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNox_MemoryCollectorAI.h"

#include "Characters/Enemy/CNoxEnemy_Animinstance.h"
#include "Global.h"

ACNox_MemoryCollectorAI::ACNox_MemoryCollectorAI()
{
	EnemyType = EEnemyType::MemoryCollector;
}

void ACNox_MemoryCollectorAI::BeginPlay()
{
	Super::BeginPlay();
}
