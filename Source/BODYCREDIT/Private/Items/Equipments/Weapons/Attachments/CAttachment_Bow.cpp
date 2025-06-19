#include "Items/Equipments/Weapons/Attachments/CAttachment_Bow.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Items/Equipments/Weapons/AnimInstances/CAnimInstance_Bow.h"
#include "Characters/CNox.h"
#include "GameFramework/PlayerController.h"
#include "Items/Equipments/Weapons/AddOns/CAddOn_Arrow.h"

float* ACAttachment_Bow::GetBend()
{
	return Cast<UCAnimInstance_Bow>(SkeletalMeshComp->GetAnimInstance())->GetBend();
}

ACAttachment_Bow::ACAttachment_Bow()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh", Root);
	SkeletalMeshComp->SetupAttachment(Root);
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &PoseableMesh, "PoseableMesh", RootComponent);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/Scifi_Arsenal_Vol3_1/Skeletal_Meshes/Sci-fi_Bow/SK_Sci-fi_Bow.SK_Sci-fi_Bow'");
	SkeletalMeshComp->SetSkeletalMesh(mesh);
	SkeletalMeshComp->SetCollisionProfileName("NoCollision");

	TSubclassOf<UCAnimInstance_Bow> animInstance;
	CHelpers::GetClass<UCAnimInstance_Bow>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/Items/Equipments/Weapons/Bow/BP_CAnimInstance_Bow.BP_CAnimInstance_Bow_C'");
	SkeletalMeshComp->SetAnimInstanceClass(animInstance);
}

void ACAttachment_Bow::BeginPlay()
{
	// AttachTo("Holster_Bow");
	SkeletalMeshComp->SetupAttachment(RootComponent);
	// SkeletalMeshComp->SetVisibility(false);
	// PoseableMesh->SetVisibility(true);

	PoseableMesh->SetSkinnedAsset(SkeletalMeshComp->GetSkeletalMeshAsset());
	// PoseableMesh->SetSkeletalMesh(SkeletalMeshComp->GetSkeletalMeshAsset());
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMeshComp);

	Super::BeginPlay();
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

void ACAttachment_Bow::CreateArrow()
{
	if (OwnerCharacter->GetWorld()->bIsTearingDown == true) return;

	FTransform transform;
	ACAddOn_Arrow* arrow = OwnerCharacter->GetWorld()->SpawnActorDeferred<ACAddOn_Arrow>(ArrowClass, transform, GetOwner(), NULL, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	CheckNull(arrow);

	arrow->AddIgnoreActor(OwnerCharacter);
	arrow->AddIgnoreActor(this);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Arrow");
	arrow->SetHidden(true);

	Arrows.Add(arrow);
	UGameplayStatics::FinishSpawningActor(arrow, transform);
}
