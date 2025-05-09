#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CNox.generated.h"

UCLASS()
class BODYCREDIT_API ACNox : public ACharacter
{
	GENERATED_BODY()

public:
	ACNox();

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamID = 1;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};