#include "Characters/CNox.h"

#include "AIController.h"
#include "Global.h"

ACNox::ACNox()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACNox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACNox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACNox::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACNox::TakeDamage(float Amount)
{
}

void ACNox::Die()
{
}
