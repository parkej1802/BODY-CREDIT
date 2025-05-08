#include "Characters/CNoxAnimInstance.h"
#include "Global.h"
#include "Characters/CNox.h"
#include "GameFramework/CharacterMovementComponent.h"

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
		// 방향 보간 ( -180 ~ +180 )
		FRotator rot1 = OwnerCharacter->GetVelocity().ToOrientationRotator();
		FRotator rot2 = OwnerCharacter->GetControlRotation();
		FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rot1, rot2);
		PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
		Direction = PrevRotation.Yaw;

		// 캐릭터 이동 속도 ( 0 ~ )
		Speed = OwnerCharacter->GetVelocity().Size2D();

		// 캐릭터 점프 중인지
		bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();
	}

}
