#include "Items/Equipments/Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Items/Equipments/Weapons/AnimInstances/CAnimInstance_Bow.h"
#include "Characters/CNox.h"
#include "GameFramework/PlayerController.h"

float* ACAttachment_Bow::GetBend()
{
	return Cast<UCAnimInstance_Bow>(SkeletalMesh->GetAnimInstance())->GetBend();

}

ACAttachment_Bow::ACAttachment_Bow()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh", Root);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/Scifi_Arsenal_Vol3_1/Skeletal_Meshes/Sci-fi_Bow/SK_Sci-fi_Bow.SK_Sci-fi_Bow'");
	SkeletalMesh->SetSkeletalMesh(mesh);
	SkeletalMesh->SetCollisionProfileName("NoCollision");

	TSubclassOf<UCAnimInstance_Bow> animInstance;
	CHelpers::GetClass<UCAnimInstance_Bow>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/Items/Equipments/Weapons/Bow/BP_CAnimInstance_Bow.BP_CAnimInstance_Bow_C'");
	SkeletalMesh->SetAnimInstanceClass(animInstance);

}

void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();

	//AttachTo("Holster_Bow");

	SkeletalMesh->SetVisibility(false);
	PoseableMesh->SetVisibility(false);

	PoseableMesh->SetSkeletalMesh(SkeletalMesh->GetSkeletalMeshAsset());
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
}

void ACAttachment_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);

}

void ACAttachment_Bow::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();

	AttachTo("Hand_Bow");

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	if (!!controller)
	{
		OriginViewPitchRange.X = controller->PlayerCameraManager->ViewPitchMin;
		OriginViewPitchRange.Y = controller->PlayerCameraManager->ViewPitchMax;

		controller->PlayerCameraManager->ViewPitchMin = ViewPitchRange.X;
		controller->PlayerCameraManager->ViewPitchMax = ViewPitchRange.Y;
	}

}

void ACAttachment_Bow::OnUnequip_Implementation()
{
	Super::OnUnequip_Implementation();

	AttachTo("Holster_Bow");

	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
	if (!!controller)
	{
		controller->PlayerCameraManager->ViewPitchMin = OriginViewPitchRange.X;
		controller->PlayerCameraManager->ViewPitchMax = OriginViewPitchRange.Y;
	}

}
