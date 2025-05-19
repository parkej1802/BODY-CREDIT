#include "Components/Enemy/CNox_BehaviorComponent.h"

// #include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/AI/Keys/CBBK_MemoryFragment.h"
#include "Data/CMemoryData.h"
#include "Patrol/CPatrolRoute.h"
#include "Utilities/CLog.h"

UCNox_BehaviorComponent::UCNox_BehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCNox_BehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCNox_BehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCNox_BehaviorComponent::SetEnemyType(EEnemyType InType)
{
	Blackboard->SetValueAsEnum(AIStateTypeKey, (uint8)InType);
}

void UCNox_BehaviorComponent::SetTarget(class ACNox* InTarget)
{
	Blackboard->SetValueAsObject(TargetPlayer, InTarget);
}

void UCNox_BehaviorComponent::SetPatrolRoute(class ACPatrolRoute* InPatrolRoute)
{
	Blackboard->SetValueAsObject(PatrolRoute, InPatrolRoute);
}

void UCNox_BehaviorComponent::SetGrenadeEnded(bool InbEndedAnim)
{
	// CLog::Log(FString::Printf(TEXT("SetGrenadeEnded: %d"), InbEndedAnim));
	Blackboard->SetValueAsBool(GrenadeKey, InbEndedAnim);
}

void UCNox_BehaviorComponent::SetEquipShield(bool bInEquipShield)
{
	// CLog::Log(FString::Printf(TEXT("SetEquipShield: %s , %d"), *EquipShieldKey.ToString(), bInEquipShield));
	Blackboard->SetValueAsBool(EquipShieldKey, bInEquipShield);
}

void UCNox_BehaviorComponent::SetShieldEnded(bool InbEndedAnim)
{
	Blackboard->SetValueAsBool(ShieldKey, InbEndedAnim);
}

void UCNox_BehaviorComponent::SetHealFlag(bool bInHealFlag)
{
	Blackboard->SetValueAsBool(HealFlagKey, bInHealFlag);
}

void UCNox_BehaviorComponent::SetPatrolLocation(const FVector& InNewLoc)
{
	Blackboard->SetValueAsVector(PatrolLocationKey, InNewLoc);
}

FVector UCNox_BehaviorComponent::GetPatrolLocation()
{
	return Blackboard->GetValueAsVector(PatrolLocationKey);
}

void UCNox_BehaviorComponent::SetMemoryTarget(FMemoryFragment InMemoryTarget)
{
	uint8* RawData = Blackboard->GetKeyRawData(MemoryTargetKey);
	UCBBK_MemoryFragment::SetValue(RawData, InMemoryTarget);
}

FMemoryFragment UCNox_BehaviorComponent::GetMemoryTarget()
{
	uint8* RawData = Blackboard->GetKeyRawData(MemoryTargetKey);
	return UCBBK_MemoryFragment::GetValue(RawData);
}

void UCNox_BehaviorComponent::SetHasMemoryTarget(bool bInHasMemoryTarget)
{
	Blackboard->SetValueAsBool(HasMemoryTargetKey, bInHasMemoryTarget);
}

bool UCNox_BehaviorComponent::GetHasMemoryTarget()
{
	return Blackboard->GetValueAsBool(HasMemoryTargetKey);
}
