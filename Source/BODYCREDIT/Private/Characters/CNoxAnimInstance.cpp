#include "Characters/CNoxAnimInstance.h"
#include "Global.h"
#include "Characters/CNox.h"

void UCNoxAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACNox>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

}

void UCNoxAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter)
	{
		FRotator rot1 = OwnerCharacter->GetVelocity().ToOrientationRotator();
		FRotator rot2 = OwnerCharacter->GetControlRotation();
		FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rot1, rot2);
		PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
		Direction = PrevRotation.Yaw;

		Speed = OwnerCharacter->GetVelocity().Size2D();
	}

}
