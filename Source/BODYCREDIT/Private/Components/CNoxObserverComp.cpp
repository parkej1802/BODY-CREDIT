#include "Components/CNoxObserverComp.h"

#include "Characters/CNox_Runner.h"
#include "Data/CMemoryData.h"

UCNoxObserverComp::UCNoxObserverComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.15f;
}

void UCNoxObserverComp::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACNox_Runner>(GetOwner());

	LastRecordedLocation = OwnerCharacter->GetActorLocation();
}


void UCNoxObserverComp::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	{
		// Intrusion
		const FVector CurrentLocation = OwnerCharacter->GetActorLocation();
		const float DistMovedSq = FVector::DistSquared(CurrentLocation, LastRecordedLocation);

		if (DistMovedSq <= FMath::Square(PositionTolerance))
		{
			AccumTime += DeltaTime;
			if (AccumTime >= RequiredStayTime)
			{
				OwnerCharacter->MakeMemoryPiece(EMemoryTriggerType::Stay);
				AccumTime = 0.f;
			}
		}
		else
		{
			// 움직였으므로 초기화
			AccumTime = 0.f;
			LastRecordedLocation = CurrentLocation;
		}
	}

	{
		// Attack
		if (bIsAttackingCooldown && CheckAndNotifyIfWithinAttackCooldown())
		{
			HandleAttackExpired();
			bIsAttackingCooldown = false;
			OwnerCharacter->GetWorldTimerManager().SetTimer(AttackCooldownHandle, [this]()
			{
				bIsAttackingCooldown = true;
			}, AttackCooldownSeconds, false);
		}
	}

	{
		// Looting
		if (bIsLootingCooldown && CheckAndNotifyIfWithinLootingCooldown())
		{
			HandleLootingExpired();
			bIsLootingCooldown = false;
			OwnerCharacter->GetWorldTimerManager().SetTimer(LootingCooldownHandle, [this]()
			{
				bIsLootingCooldown = true;
			}, LootingCooldownSeconds, false);
		}
	}
}

void UCNoxObserverComp::HandleAttackExpired()
{
	OwnerCharacter->MakeMemoryPiece(EMemoryTriggerType::Combat);
	LastAttackTime = -FLT_MAX;
}

bool UCNoxObserverComp::CheckAndNotifyIfWithinAttackCooldown() const
{
	const float Now = GetWorld()->GetTimeSeconds();
	if ((Now - LastAttackTime) <= AttackCooldownSeconds)
	{
		return true;
	}
	return false;
}

void UCNoxObserverComp::HandleLootingExpired()
{
	OwnerCharacter->MakeMemoryPiece(EMemoryTriggerType::Looting);
	LastLootingTime = -FLT_MAX;
}

bool UCNoxObserverComp::CheckAndNotifyIfWithinLootingCooldown() const
{
	const float Now = GetWorld()->GetTimeSeconds();
	if ((Now - LastLootingTime) <= LootingCooldownSeconds)
	{
		return true;
	}
	return false;
}
