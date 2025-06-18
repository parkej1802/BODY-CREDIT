#include "Characters/Player/CAnimInstance_Arms.h"
#include "Global.h"
#include "Characters/Player/CNox_Player.h"

void UCAnimInstance_Arms::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACNox_Player>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	Weapon = CHelpers::GetComponent<UCGunComponent>(OwnerCharacter);
	CheckNull(Weapon);

	Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCAnimInstance_Arms::OnWeaponTypeChanged);
}

void UCAnimInstance_Arms::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	LeftHandTransform = Weapon->GetLeftHandTransform();
}

void UCAnimInstance_Arms::OnWeaponTypeChanged(EGunType InPrevType, EGunType InNewType)
{
	WeaponType = InNewType;
}
