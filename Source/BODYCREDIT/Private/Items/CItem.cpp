#include "Items/CItem.h"
#include "Global.h"
#include "Components/SceneComponent.h"
#include "Characters/CNox.h"

ACItem::ACItem()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACItem::BeginPlay()
{
	Super::BeginPlay();

}

void ACItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACItem::OnPickUpItem(ACNox* InNewOwnerCharacter)
{
	SetOwner(InNewOwnerCharacter);
	OwnerCharacter = Cast<ACNox>(InNewOwnerCharacter);

}
