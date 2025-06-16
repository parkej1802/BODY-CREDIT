#include "Characters/Enemy/CNox_EBase.h"

#include "global.h"
#include "Characters/Enemy/CNoxEnemy_Animinstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Enemy/CFSMComponent.h"
#include "Characters/Enemy/AI/CEnemyController.h"
#include "Components/AudioComponent.h"
#include "Components/Enemy/CNoxEnemyHPComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Sound/SoundCue.h"

#pragma region Init
ACNox_EBase::ACNox_EBase()
{
	InitComp();
	Tags.Add("Enemy");
}

void ACNox_EBase::InitComp()
{
	TeamID = 2;
	CHelpers::CreateComponent<UAudioComponent>(this, &SoundComponent, "SoundComponent", RootComponent);

	GetCapsuleComponent()->SetCollisionProfileName(FName("Enemy"));
	{
		CHelpers::GetClass(&AIControllerClass, TEXT("/Game/Characters/Enemy/AI/BP_NoxController.BP_NoxController_C"));
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}
	{
		// 목표에 거리가 가까워지면서 속도가 줄어드는 현상 방지
		// GetCharacterMovement()->GetNavMovementProperties()->bUseFixedBrakingDistanceForPaths = false;
		GetCharacterMovement()->GetNavMovementProperties()->FixedPathBrakingDistance = 0;
	}
	{
		CHelpers::CreateActorComponent<UCNoxEnemyHPComponent>(this, &HPComp, "HPComp");
		CHelpers::CreateActorComponent<UCFSMComponent>(this, &FSMComp, "FSMComp");
	}
}

void ACNox_EBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	EnemyController = Cast<ACEnemyController>(NewController);
}

void ACNox_EBase::BeginPlay()
{
	Super::BeginPlay();

	if (auto Anim = GetMesh()->GetAnimInstance())
	{
		EnemyAnim = Cast<UCNoxEnemy_Animinstance>(Anim);
		EnemyAnim->SetEnemy(this);
	}

	if (HPComp) HPComp->SetEnemy(this);

	if (FSMComp) FSMComp->InitializeFSM(this);

	SoundComponent->SetSound(IdleSoundCue);
}
#pragma endregion

#pragma region Tick
void ACNox_EBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FSMComp && !bExtractSucceed) FSMComp->UpdateState();

	// if (Target)
	// {
	// 	float Dot = FVector::DotProduct(GetActorForwardVector(), (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal());
	// 	CLog::Print(Dot);		
	// }
	if (bDebug)
	{
		if (FSMComp) // Print Current State
		{
			FString myState = UEnum::GetValueOrBitfieldAsString(FSMComp->GetEnemyState());
			DrawDebugString(GetWorld(), GetActorLocation(), myState, nullptr, FColor::Yellow, 0);
			myState = UEnum::GetValueOrBitfieldAsString(FSMComp->GetCombatState());
			DrawDebugString(
				GetWorld(), FVector(GetActorLocation().X, GetActorLocation().Y,
				                    GetActorLocation().Z - 50), myState, nullptr, FColor::Yellow, 0);
		}
		if (HPComp)
		{
			FString myHP = FString::Printf(TEXT("%.2f"), HPComp->GetHealthPercent());
			DrawDebugString(
				GetWorld(), FVector(GetActorLocation().X, GetActorLocation().Y,
				                    GetActorLocation().Z - 100), myHP, nullptr, FColor::Red, 0);
		}
	}
}
#pragma endregion

#pragma region Apply Damage
void ACNox_EBase::SetApplyDamage(AActor* DamagedPlayer, const float DamageAmout)
{
	UGameplayStatics::ApplyDamage(DamagedPlayer, DamageAmout, EnemyController, this, UDamageType::StaticClass());
}
#pragma endregion

#pragma region Set Target
void ACNox_EBase::SetTarget(ACNox* InTarget)
{
	if (HPComp->GetHealthPercent() <= FLT_MIN) return;

	Target = InTarget;
	Target ? FSMComp->SetEnemyState(EEnemyState::Sense) : FSMComp->SetEnemyState(EEnemyState::IDLE);
}
#pragma endregion

#pragma region Set Movement Speed
void ACNox_EBase::SetMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed)
{
	float newSpeed = 0.f, newAccelSpeed = 0.f;
	GetNewMovementSpeed(InMovementSpeed, newSpeed, newAccelSpeed);
	GetCharacterMovement()->MaxWalkSpeed = newSpeed;
	GetCharacterMovement()->MaxAcceleration = newAccelSpeed;
}
#pragma endregion

#pragma region Attacking
void ACNox_EBase::HandleAttack()
{
	EnemyAnim->PlayAttackMontage();
}

bool ACNox_EBase::IsAttacking()
{
	return EnemyAnim->IsAttacking();
}
#pragma endregion

#pragma region BroadCast To Near Enemy
void ACNox_EBase::SetTargetCallByDelegate(ACNox* InTarget)
{
	EnemyController->SetTargetPlayer(InTarget);
}
#pragma endregion

#pragma region Hitting
void ACNox_EBase::HandleHit(const int32 sectionIdx)
{
	EnemyAnim->PlayHitMontage(sectionIdx);
}

bool ACNox_EBase::IsHitting()
{
	return EnemyAnim->IsHitting();
}

void ACNox_EBase::ResetVal() const
{
	FSMComp->ResetVal(EnemyType);
}
#pragma endregion

#pragma region Die
void ACNox_EBase::HandleDie(const int32 sectionIdx)
{
	GetCapsuleComponent()->SetCollisionProfileName(FName("EnemyDie"));
	if (EnemyAnim) EnemyAnim->PlayDieMontage(sectionIdx);
	EnemyController->PerceptionDeactive();
}
#pragma endregion

#pragma region Heal Hp (Medic)
void ACNox_EBase::HealHP()
{
	HPComp->HealHP(HealAmount);
}
#pragma endregion

#pragma region Check Player In Forward Degree
bool ACNox_EBase::IsPlayerInForwardDegree(const float InForwardRange, const float InDegree)
{
	if (!Target) return false;

	const FVector MyLocation = GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	const float DistanceSquared = FVector::DistSquared(MyLocation, TargetLocation);

	// 거리 제곱으로 비교하여 제곱근 연산 방지
	if (DistanceSquared > FMath::Square(InForwardRange))
	{
		CLog::Log("Out Of Range");
		return false;
	}

	const FVector Forward2D = FVector(GetActorForwardVector().X, GetActorForwardVector().Y, 0.f).GetSafeNormal();
	TargetLocation.Z = 0.f;
	const FVector ToTarget2D = FVector(TargetLocation - MyLocation).GetSafeNormal2D();

	// 내적 계산 후 각도 변환
	const float Dot = FMath::Clamp(FVector::DotProduct(Forward2D, ToTarget2D), -1.f, 1.f);
	return FMath::RadiansToDegrees(FMath::Acos(Dot)) <= InDegree;
}
#pragma endregion

#pragma region FSM Set State
void ACNox_EBase::SetEnemyState(EEnemyState NewState)
{
	FSMComp->SetEnemyState(NewState);
}

void ACNox_EBase::SetCombatState(ECombatState NewCombatState)
{
	FSMComp->SetCombatState(NewCombatState);
}
#pragma endregion

#pragma region FSM Skill Cool Downs
void ACNox_EBase::UpdateSkillCoolDowns(ESkillCoolDown Skill, float DeltaTime)
{
	FSMComp->UpdateSkillCoolDowns(Skill, DeltaTime);
}

bool ACNox_EBase::IsSkillReady(ESkillCoolDown Skill) const
{
	return FSMComp->IsSkillReady(Skill);
}

void ACNox_EBase::UsingSkill(ESkillCoolDown Skill)
{
	FSMComp->UsingSkill(Skill);
}

bool ACNox_EBase::RotateToTarget(const float DeltaTime, const FTransform& CurTrans, const FVector& TargetLoc,
                                 float InteropSpeed)
{
	// 1. 현재 위치와 목표 위치를 2D로 변환
	const FVector CurrentLocation = CurTrans.GetLocation();
	const FVector DirectionToTarget = (TargetLoc - CurrentLocation).GetSafeNormal2D();

	// 2. 목표 회전값 계산
	const FRotator TargetRotation = DirectionToTarget.Rotation();

	// 3. 현재 회전값 가져오기
	const FRotator CurrentRotation = CurTrans.GetRotation().Rotator();

	// 4. Yaw만 보간
	const float NewYaw = FMath::FInterpTo(
		CurrentRotation.Yaw,
		TargetRotation.Yaw,
		DeltaTime,
		InteropSpeed
	);

	// 5. 새로운 회전값 설정 (Pitch와 Roll은 유지)
	const FRotator NewRotation(CurrentRotation.Pitch, NewYaw, CurrentRotation.Roll);
	SetActorRotation(NewRotation);

	return true;
}

void ACNox_EBase::SetRotateToTarget()
{
	// 1. 현재 회전값 가져오기
	const FRotator CurrentRotation = GetActorRotation();

	// 2. 목표 방향 계산
	const FVector DirectionToTarget = (GetTarget()->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
	const float TargetYaw = DirectionToTarget.Rotation().Yaw;

	// 3. 새로운 회전값 설정 (Pitch와 Roll은 유지)
	const FRotator NewRotation(CurrentRotation.Pitch, TargetYaw, CurrentRotation.Roll);
	SetActorRotation(NewRotation);
}
#pragma endregion

#pragma region Extract Call Function
void ACNox_EBase::ExtractCallFunction(ACNox* InTarget)
{
	RetentionTime = 0.f; // 타겟 잃어버림 방지용, 컨트롤러에서 RetentionTime으로 잊게 해놈
	SetTarget(InTarget);
}
#pragma endregion

#pragma region Sound
void ACNox_EBase::PlayIdleSound()
{
	if (IdleSoundCue && SoundComponent->Sound != IdleSoundCue)
	{
		SoundComponent->SetSound(IdleSoundCue);
		SoundComponent->Play();
	}
}

void ACNox_EBase::PlaySenseSound()
{
	if (SenseSoundCue && SoundComponent->Sound != SenseSoundCue)
	{
		SoundComponent->SetSound(SenseSoundCue);
		SoundComponent->Play();
	}
}

void ACNox_EBase::PlayAttackSound()
{
	if (AttackSoundCue && SoundComponent->Sound != AttackSoundCue)
	{
		SoundComponent->SetSound(AttackSoundCue);
		SoundComponent->Play();
	}
}

void ACNox_EBase::PlayGrenadeSound()
{
	if (GrenadeSoundCue && SoundComponent->Sound != GrenadeSoundCue)
	{
		SoundComponent->SetSound(GrenadeSoundCue);
		SoundComponent->Play();
	}
}

void ACNox_EBase::PlayHealSound()
{
	if (HealSoundCue && SoundComponent->Sound != HealSoundCue)
	{
		SoundComponent->SetSound(HealSoundCue);
		SoundComponent->Play();
	}
}

void ACNox_EBase::PlayHitSound()
{
	if (HitSoundCue && SoundComponent->Sound != HitSoundCue)
	{
		SoundComponent->SetSound(HitSoundCue);
		SoundComponent->Play();
	}
}

void ACNox_EBase::PlayDieSound()
{
	if (DieSoundCue && SoundComponent->Sound != DieSoundCue)
	{
		SoundComponent->SetSound(DieSoundCue);
		SoundComponent->Play();
	}
}
#pragma endregion
