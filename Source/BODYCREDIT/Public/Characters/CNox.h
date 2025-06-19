#pragma once

#include "CoreMinimal.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/ICharacter.h"
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
	UPROPERTY()
	class ACMainGM* MainGM;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void PlayHitStop(float InHitStopTime);
	void PlayLaunchCharacter(float InLaunchPower);
};