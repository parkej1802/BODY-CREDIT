#include "Items/Equipments/Weapons/CWeapon_Attachment.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Characters/CNox.h"
#include "Components/ShapeComponent.h"

ACWeapon_Attachment::ACWeapon_Attachment()
{
	PrimaryActorTick.bCanEverTick = true;
	
}

void ACWeapon_Attachment::BeginPlay()
{
	OwnerCharacter = Cast<ACNox>(GetOwner());

	TArray<USceneComponent*> children;
	Root->GetChildrenComponents(true, children);
	for (USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);

		if (!!shape)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACWeapon_Attachment::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACWeapon_Attachment::OnComponentEndOverlap);

			Collisions.Add(shape);
		}
		OffCollisions();
	}

	Super::BeginPlay();

}

void ACWeapon_Attachment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACWeapon_Attachment::OnCollisions()
{
	if (OnWeaponAttachmentBeginCollision.IsBound())
		OnWeaponAttachmentBeginCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void ACWeapon_Attachment::OffCollisions()
{
	if (OnWeaponAttachmentEndCollision.IsBound())
		OnWeaponAttachmentEndCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

}

void ACWeapon_Attachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());

	if (OnWeaponAttachmentBeginOverlap.IsBound())
		OnWeaponAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACNox>(OtherActor));

}

void ACWeapon_Attachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());

	if (OnWeaponAttachmentEndOverlap.IsBound())
		OnWeaponAttachmentEndOverlap.Broadcast(OwnerCharacter, Cast<ACNox>(OtherActor));

}

void ACWeapon_Attachment::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);

}

void ACWeapon_Attachment::AttachToCollision(FName InCollisionName)
{
	for (UShapeComponent* collision : Collisions)
	{
		if (collision->GetName() == InCollisionName.ToString())
		{
			collision->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InCollisionName);

			return;
		}
	}

}

void ACWeapon_Attachment::OnPickUpItem(ACNox* InNewOwnerCharacter)
{
	Super::OnPickUpItem(InNewOwnerCharacter);

	if (OnWeaponAttachmentEndCollision.IsBound())
		OnWeaponAttachmentEndCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}
