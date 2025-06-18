// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/Lootable/LootableInterface.h"
#include "Lootable_Base.generated.h"

UCLASS()
class BODYCREDIT_API ALootable_Base : public AActor, public ILootableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootable_Base();

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
	class UBoxComponent* BoxComp;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereComponent;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAC_LootingInventoryComponent* LootInventoryComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UWidgetComponent* InteractWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;

	virtual class UAC_LootingInventoryComponent* GetLootInventoryComponent_Implementation() const override;

	/*UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
};
