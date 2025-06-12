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
	Hit,
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

enum class ESkillCoolDown
{
	Melee,    // 근접 스킬
	Ranged,   // 원거리 스킬
	Heal,	  // 힐
	Grenade,  // 수류탄
	Beam,	  // 레이저
	WavePulse,// 충격파
};
