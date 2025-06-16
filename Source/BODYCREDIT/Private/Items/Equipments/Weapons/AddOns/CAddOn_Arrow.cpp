#include "Items/Equipments/Weapons/AddOns/CAddOn_Arrow.h"
#include "Global.h"
#include "Characters/CNox.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACAddOn_Arrow::ACAddOn_Arrow()
{
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule");

	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Projectile, "Projectile");

	Projectile->ProjectileGravityScale = 0;
	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;
	Capsule->SetCollisionProfileName("BlockAll");

}

void ACAddOn_Arrow::BeginPlay()
{
	Super::BeginPlay();

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->OnComponentHit.AddDynamic(this, &ACAddOn_Arrow::OnComponentHit);

	Projectile->Deactivate();

}

void ACAddOn_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACAddOn_Arrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (OnEndPlay.IsBound())
		OnEndPlay.Broadcast(this);

}

void ACAddOn_Arrow::Shoot(const FVector& InForward)
{
	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->Activate();

	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void ACAddOn_Arrow::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SetLifeSpan(LifeSpanAfterCollision);

	AttachToComponent(OtherComp, FAttachmentTransformRules::KeepWorldTransform);

	for (AActor* actor : Ignores)
		CheckTrue(actor == OtherActor);

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ACNox* character = Cast<ACNox>(OtherActor);
	if (!!character && OnHit.IsBound())
		OnHit.Broadcast(this, character);

	// 2. 파티클 스폰 (BP_ArrowVanishFX는 파티클 시스템)
	if (ArrowVanishFX) // NiagaraSystem 사용 예시
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			ArrowVanishFX,
			Hit.ImpactPoint // 충돌 위치에 이펙트 생성
		);
	}

	// [3] 사운드 이펙트 재생
	if (ArrowVanishSFX)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ArrowVanishSFX,
			Hit.ImpactPoint
		);
	}
}
