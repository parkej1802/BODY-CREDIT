// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_LootingInventoryComponent.h"

// Sets default values for this component's properties
UAC_LootingInventoryComponent::UAC_LootingInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAC_LootingInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	PreAddItem();
	
}


// Called every frame
void UAC_LootingInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// GEngine->AddOnScreenDebugMessage(5, 1.f, FColor::Blue, FString::Printf(TEXT("IsDirty : %d"), IsDirty));
}

