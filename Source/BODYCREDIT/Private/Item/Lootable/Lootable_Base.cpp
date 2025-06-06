// Fill out your copyright notice in the Description page of Project Settings.


#include "Lootable_Base.h"
#include "../Public/AC_LootingInventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Session/NetGameInstance.h"

// Sets default values
ALootable_Base::ALootable_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);

	LootInventoryComp = CreateDefaultSubobject<UAC_LootingInventoryComponent>(TEXT("InventoryComp"));
}

// Called when the game starts or when spawned
void ALootable_Base::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALootable_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAC_LootingInventoryComponent* ALootable_Base::GetLootInventoryComponent_Implementation() const
{
	return LootInventoryComp;
}

