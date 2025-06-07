#include "Characters/CNoxAnimInstance.h"
#include "Global.h"
#include "Characters/CNox.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CMovementComponent.h"
#include "Items/Equipments/Weapons/CWeapon_SubAction.h"

void UCNoxAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACNox>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	if (Weapon)
		Weapon->OnWeaponTypeChange.AddDynamic(this, &UCNoxAnimInstance::OnWeaponTypeChanged);
	
}

void UCNoxAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter)
	{
		// 캐릭터 이동 속도 ( 0 ~ )
		Speed = OwnerCharacter->GetVelocity().Size2D();

		// 방향 보간 ( -180 ~ +180 )
		FRotator rot1 = OwnerCharacter->GetVelocity().ToOrientationRotator();
		FRotator rot2 = OwnerCharacter->GetControlRotation();
		FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rot1, rot2);
		PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
		Direction = PrevRotation.Yaw;

		bSprint = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter)->IsSprint();

		bCrouch = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter)->IsCrouch();

		// 캐릭터 점프 중인지
		bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling();

		CheckNull(Weapon);

		if (Weapon->GetSubAction())
		{
			bAiming = true;
			bAiming &= WeaponType == EWeaponType::BOW;
			bAiming &= Weapon->GetSubAction()->GetInAction();
		}

	}

}

void UCNoxAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;

}