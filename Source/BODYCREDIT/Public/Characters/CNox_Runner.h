#pragma once

#include "CoreMinimal.h"
#include "Characters/CNox.h"
#include "CNox_Runner.generated.h"

UCLASS()
class BODYCREDIT_API ACNox_Runner : public ACNox
{
	GENERATED_BODY()
	
public:
	ACNox_Runner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
