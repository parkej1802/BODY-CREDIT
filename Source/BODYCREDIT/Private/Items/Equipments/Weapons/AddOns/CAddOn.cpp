#include "Items/Equipments/Weapons/AddOns/CAddOn.h"
#include "Global.h"
#include "Characters/CNox.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACAddOn::ACAddOn()
{
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	Projectile->ProjectileGravityScale = 0;
	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;
	Capsule->SetCollisionProfileName("BlockAll");

}

void ACAddOn::BeginPlay()
{
	Super::BeginPlay();

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->OnComponentHit.AddDynamic(this, &ACAddOn::OnComponentHit);

	Projectile->Deactivate();
	
}

void ACAddOn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACAddOn::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (OnEndPlay.IsBound())
		OnEndPlay.Broadcast(this);

}

void ACAddOn::Shoot(const FVector& InForward)
{
	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->Activate();

	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void ACAddOn::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SetLifeSpan(LifeSpanAfterCollision);

	for (AActor* actor : Ignores)
		CheckTrue(actor == OtherActor);

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ACNox* character = Cast<ACNox>(OtherActor);
	if (!!character && OnHit.IsBound())
		OnHit.Broadcast(this, character);

}

