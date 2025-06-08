// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemDT.h"
#include "Engine/DataTable.h"
#include "ItemObject.h"
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
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Width = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Height = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ItemDataTable;

	/*UPROPERTY()
	FItemData ItemData = FItemData();*/
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class UMaterialInterface* Icon;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class UMaterialInterface* RotatedIcon;

	UPROPERTY()
	FString ContextString = TEXT("Item Data");
public:
	UFUNCTION()
    void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY()
	class UItemObject* ItemObject;

	//UItemObject* GetDefaultItemObject();
	void GetDefaultItemObject();

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ACMainGM* GameMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAC_LootingInventoryComponent* LootInventoryComp;

//#if WITH_EDITOR
//	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
//#endif

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategy")
	class UItemStrategy* ItemStrategy;

	UFUNCTION(BlueprintCallable, Category="Item")
	void SetItemStrategy(class UItemStrategy* NewStrategy);

	UFUNCTION(BlueprintCallable, Category="Item")
	void UseItem();
};
