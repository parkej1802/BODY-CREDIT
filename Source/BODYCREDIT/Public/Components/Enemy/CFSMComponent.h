#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/Enemy/CEnemyState.h"
#include "CFSMComponent.generated.h"

class ACNox_EBase;
class ICEnemyStateStrategy;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BODYCREDIT_API UCFSMComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCFSMComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	ACNox_EBase* OwnerEnemy = nullptr;
	EEnemyState CurrentEnemyState = EEnemyState::IDLE;
	ECombatState CurrentCombatState = ECombatState::Default;

	TMap<EEnemyState, TSharedPtr<ICEnemyStateStrategy>> EnemyStrategies;
	
	TMap<EEnemyState, TSharedPtr<ICEnemyStateStrategy>> CreateStrategies(EEnemyType Type);
	
public:
	void InitializeFSM(ACNox_EBase* InOwner);
	void UpdateState();

	EEnemyState GetEnemyState() const { return CurrentEnemyState; }
	ECombatState GetCombatState() const { return CurrentCombatState; }

	void SetEnemyState(EEnemyState NewState);
	void SetCombatState(ECombatState NewCombatState);
};
