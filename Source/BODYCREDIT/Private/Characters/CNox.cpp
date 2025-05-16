#include "Characters/CNox.h"

#include "AIController.h"
#include "Global.h"
#include "Games/CMainGM.h"

ACNox::ACNox()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACNox::BeginPlay()
{
	Super::BeginPlay();
	MainGM = Cast<ACMainGM>(GetWorld()->GetAuthGameMode());
}

void ACNox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACNox::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
