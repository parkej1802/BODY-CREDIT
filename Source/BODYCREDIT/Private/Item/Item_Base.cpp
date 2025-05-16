// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Item_Base.h"
#include "Item/ItemObject.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMesh.h"
#include "Characters/CNox_Runner.h"
#include "Inventory/AC_InventoryComponent.h"
#include "Item/ItemDT.h"
#include "Games/CMainGM.h"

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

	ItemObject = CreateDefaultSubobject<UItemObject>(TEXT("ItemObject"));
	

}

void AItem_Base::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Item/DT_ItemData.DT_ItemData"));

	if (ItemDataTable)
	{
		FItemData* TempData = ItemDataTable->FindRow<FItemData>(ItemName, ContextString);
		if (TempData && TempData->Mesh)
		{
			ItemData = *TempData;
			StaticMeshComp->SetStaticMesh(ItemData.Mesh);

			GetDefaultItemObject();
		}
	}
}


//#if WITH_EDITOR
//void AItem_Base::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
//{
//	Super::PostEditChangeProperty(PropertyChangedEvent);
//
//	ItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Item/DT_ItemData.DT_ItemData"));
//
//	if (ItemDataTable)
//	{
//		FItemData* TempData = ItemDataTable->FindRow<FItemData>(ItemName, ContextString);
//		if (TempData && TempData->Mesh)
//		{
//			ItemData = *TempData;
//			StaticMeshComp->SetStaticMesh(ItemData.Mesh);
//		}
//	}
//
//
//	Dimensions.X = ItemData.Dimensions.X;
//	Dimensions.Y = ItemData.Dimensions.Y;
//	Icon = ItemData.Icon;
//	RotatedIcon = ItemData.RotatedIcon;
//	ItemClass = ItemData.ItemClass;
//	
//
//	if (GEditor)
//	{
//		GEditor->RedrawAllViewports();
//	}
//}
//#endif


// Called when the game starts or when spawned
void AItem_Base::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AItem_Base::OnSphereBeginOverlap);

	GameMode = Cast<ACMainGM>(GetWorld()->GetAuthGameMode());
	
	ItemObject->ID = GameMode->GetItemIndex();
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

void AItem_Base::GetDefaultItemObject()
{
	ItemObject->Dimensions.X = ItemData.Dimensions.X;
	ItemObject->Dimensions.Y = ItemData.Dimensions.Y;
	ItemObject->Icon = ItemData.Icon;
	ItemObject->RotatedIcon = ItemData.RotatedIcon;
	ItemObject->ItemClass = ItemData.ItemClass;
}
