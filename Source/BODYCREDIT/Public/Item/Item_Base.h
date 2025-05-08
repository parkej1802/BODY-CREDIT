// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item_Base.generated.h"

UCLASS()
class BODYCREDIT_API AItem_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Width = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Height = 1.f;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* RotatedIcon;

public:
	UFUNCTION()
    void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UItemObject* ItemObject;

	virtual UItemObject* GetDefaultItemObject();


};
