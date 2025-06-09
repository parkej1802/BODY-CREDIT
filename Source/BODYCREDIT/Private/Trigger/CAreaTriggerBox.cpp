#include "Trigger/CAreaTriggerBox.h"
#include "Components/BoxComponent.h"
#include "global.h"

ACAreaTriggerBox::ACAreaTriggerBox()
{
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent<UBoxComponent>(this, &BoxComp, "BoxComp");
	BoxComp->SetCollisionProfileName(FName("Zone"));
}

void ACAreaTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACAreaTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

