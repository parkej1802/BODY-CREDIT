#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSpawnBoundaryBox.generated.h"

UCLASS()
class BODYCREDIT_API ACSpawnBoundaryBox : public AActor
{
	GENERATED_BODY()

public:
	ACSpawnBoundaryBox();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere)
	int32 floor = 1;
public:
	int32 GetFloor() const { return floor; }
	FVector GetLoc() const;
	FVector GetScale() const;
};
