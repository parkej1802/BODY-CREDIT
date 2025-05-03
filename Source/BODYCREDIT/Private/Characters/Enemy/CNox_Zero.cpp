// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemy/CNox_Zero.h"

#include "EngineUtils.h"
#include "StaticMeshAttributes.h"
#include "Components/Enemy/CNox_BehaviorComponent.h"
#include "Engine/OverlapResult.h"
#include "Patrol/CPatrolRoute.h"
#include "Utilities/CLog.h"

ACNox_Zero::ACNox_Zero()
{
	EnemyType = EEnemyType::Zero;
}

void ACNox_Zero::BeginPlay()
{
	Super::BeginPlay();
	
	GetNearPatrolRoute();
}

void ACNox_Zero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACNox_Zero::GetNearPatrolRoute()
{
	for (TActorIterator<ACPatrolRoute> It(GetWorld(), ACPatrolRoute::StaticClass()); It; ++It)
	{
		AActor* Actor = *It;

		if (!NearPatrolRoute) NearPatrolRoute = *It;
		else
		{
			if (FVector::Dist((*It)->GetActorLocation(), GetActorLocation()) <
				FVector::Dist(NearPatrolRoute->GetActorLocation(), GetActorLocation()))
			{
				NearPatrolRoute = *It;
			}
		}
	}

	BehaviorComp->SetPatrolRoute(NearPatrolRoute);
}
