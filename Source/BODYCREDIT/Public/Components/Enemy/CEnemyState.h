#pragma once

#include "CoreMinimal.h"

// Enemy Main State
UENUM(BlueprintType)
enum class EEnemyState:uint8
{
	IDLE,
	MemoryHunt,
	Sense,
	Combat,
	Die
};

// Enemy Combat Sub State
UENUM(BlueprintType)
enum class ECombatState:uint8
{
	Default,
	Heal,
	Grenade,
	Beam,
	WavePulse
};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	Cctv,
	Zero,
	MedicAndroid,
	MemoryCollector,
};

UENUM(BlueprintType)
enum class EEnemyMovementSpeed : uint8
{
	Idle,
	Walking,
	Sprinting,
	Combat
};
