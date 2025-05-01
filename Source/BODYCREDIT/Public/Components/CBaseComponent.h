#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBaseComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYCREDIT_API UCBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCBaseComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY()
	class ACNox* OwnerCharacter;

};
