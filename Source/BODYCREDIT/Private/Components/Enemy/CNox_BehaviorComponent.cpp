#include "Components/Enemy/CNox_BehaviorComponent.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "BehaviorTree/BlackboardComponent.h"
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

void UCNox_BehaviorComponent::SetPatrolRandomLocation(const FVector& InNewLoc)
{
	Blackboard->SetValueAsVector(PatrolRandomLocationKey, InNewLoc);
}

FVector UCNox_BehaviorComponent::GetPatrolRandomLocation()
{
	return Blackboard->GetValueAsVector(PatrolRandomLocationKey);
}
