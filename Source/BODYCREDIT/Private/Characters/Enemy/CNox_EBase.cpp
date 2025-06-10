#include "Characters/Enemy/CNox_EBase.h"

#include "global.h"
#include "Characters/Enemy/CNoxEnemy_Animinstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Enemy/CFSMComponent.h"
#include "Characters/Enemy/AI/CEnemyController.h"
#include "Components/Enemy/CNoxEnemyHPComponent.h"
#include "Navigation/PathFollowingComponent.h"

#pragma region Init
ACNox_EBase::ACNox_EBase()
{
	InitComp();
}

void ACNox_EBase::InitComp()
{
	TeamID = 2;
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
}
#pragma endregion

#pragma region Tick
void ACNox_EBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FSMComp) FSMComp->UpdateState();

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
	if (FVector::Dist(Target->GetActorLocation(), GetActorLocation()) > InForwardRange)
	{
		CLog::Log("Out Of Range");
		return false;
	}
	
	float Dot = FVector::DotProduct(GetActorForwardVector(), (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal());
	return Dot <= InDegree;
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
	float newYaw = (TargetLoc - CurTrans.GetLocation()).GetSafeNormal().Rotation().Yaw;
	FRotator TargetRot = FRotator(0, newYaw, 0);

	// 보간
	FRotator NewRot = FMath::RInterpTo(CurTrans.GetRotation().Rotator(), TargetRot, DeltaTime, InteropSpeed);
	SetActorRotation(NewRot);

	return true;
}
#pragma endregion
