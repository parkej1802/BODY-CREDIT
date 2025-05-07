#include "Components/Enemy/CNox_BehaviorComponent.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "BehaviorTree/BlackboardComponent.h"
#include "Patrol/CPatrolRoute.h"

UCNox_BehaviorComponent::UCNox_BehaviorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCNox_BehaviorComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCNox_BehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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
	Blackboard->SetValueAsBool(GrenadeKey, InbEndedAnim);
}

void UCNox_BehaviorComponent::SetShieldEnded(bool InbEndedAnim)
{
	Blackboard->SetValueAsBool(ShieldKey, InbEndedAnim);
}

