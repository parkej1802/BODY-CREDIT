#include "Notifies/CAnimNotify_FootStep.h"
#include "Global.h"
#include "Sound/SoundBase.h"

FString UCAnimNotify_FootStep::GetNotifyName() const
{
	return "FootStep";
	
}

void UCAnimNotify_FootStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	CheckTrue(SoundList.IsEmpty());
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundList[FMath::RandRange(0, SoundList.Num() - 1)], MeshComp->GetComponentLocation());
	
	// 2. MakeNoise 호출
	APawn* OwnerPawn = Cast<APawn>(MeshComp->GetOwner());
	if (OwnerPawn)
	{
		// Loudness: 1.0, NoiseInstigator: OwnerPawn, NoiseLocation: 현재 위치, MaxRange: 0(자동), Tag: "FootStep"
		OwnerPawn->MakeNoise(
			Loudness,
			OwnerPawn,
			MeshComp->GetComponentLocation(),
			MaxRange,
			FName("Hearing")
		);
	}
	
}
