// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item_Base.h"
#include "Item/ItemObject.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMesh.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_InventoryComponent.h"

// Sets default values
AItem_Base::AItem_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(RootComponent);

	SphereComp->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void AItem_Base::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsValid(ItemObject))
	{
		ItemObject = GetDefaultItemObject();
	}

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItem_Base::OnSphereBeginOverlap);
}

// Called every frame
void AItem_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem_Base::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACNox_Runner* PlayerCharacter = Cast<ACNox_Runner>(OtherActor)) 
	{
		if (PlayerCharacter->InventoryComp->TryAddItem(ItemObject)) 
		{
			Destroy();
		}
	}

}

UItemObject* AItem_Base::GetDefaultItemObject()
{
	return NewObject<UItemObject>(this);
}

