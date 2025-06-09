#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CVent.generated.h"

UCLASS()
class BODYCREDIT_API ACVent : public AActor
{
	GENERATED_BODY()
	
public:	
	ACVent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Vent")
	USceneComponent* Root;
	
public:
	FVector GetLocation() const;
};
