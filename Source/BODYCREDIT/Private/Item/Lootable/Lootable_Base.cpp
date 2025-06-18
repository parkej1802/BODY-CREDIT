// Fill out your copyright notice in the Description page of Project Settings.


#include "Lootable_Base.h"
#include "../Public/AC_LootingInventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Session/NetGameInstance.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Characters/CNox_Runner.h"

// Sets default values
ALootable_Base::ALootable_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);

	/*SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);*/

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(RootComponent);

	LootInventoryComp = CreateDefaultSubobject<UAC_LootingInventoryComponent>(TEXT("InventoryComp"));

	// SphereComponent->SetSphereRadius(250.f);
}

// Called when the game starts or when spawned
void ALootable_Base::BeginPlay()
{
	Super::BeginPlay();

	/*InteractWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractWidget->SetVisibility(false);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ALootable_Base::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ALootable_Base::OnSphereEndOverlap);*/
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

//void ALootable_Base::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
//	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (Cast<ACNox_Runner>(OtherActor))
//	{
//		InteractWidget->SetVisibility(true);
//	}
//}
//
//void ALootable_Base::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
//	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if (Cast<ACNox_Runner>(OtherActor))
//	{
//		InteractWidget->SetVisibility(false);
//	}
//}