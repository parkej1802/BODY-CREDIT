#pragma once

#include "CoreMinimal.h"
#include "Item/Item_Base.h"
#include "GameFramework/Actor.h"
#include "CItem.generated.h"

UCLASS()
class BODYCREDIT_API ACItem : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	class USceneComponent* Root;
	
public:	
	ACItem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	virtual void OnPickUpItem(class ACNox* InNewOwnerCharacter);

protected:
	UPROPERTY()
	class ACNox* OwnerCharacter;

};
