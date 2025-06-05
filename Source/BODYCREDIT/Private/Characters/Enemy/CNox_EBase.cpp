#include "Characters/Enemy/CNox_EBase.h"

#include "global.h"
#include "Characters/Enemy/CNoxEnemy_Animinstance.h"
#include "Components/Enemy/CNox_BehaviorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/Enemy/CFSMComponent.h"
#include "Characters/Enemy/AI/CEnemyController.h"
#include "Components/Enemy/CNoxEnemyHPComponent.h"
#include "Navigation/PathFollowingComponent.h"

ACNox_EBase::ACNox_EBase()
{
	TeamID = 2;

	CHelpers::GetClass(&AIControllerClass, TEXT("/Game/Characters/Enemy/AI/BP_NoxController.BP_NoxController_C"));
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 목표에 거리가 가까워지면서 속도가 줄어드는 현상 방지
	// GetCharacterMovement()->GetNavMovementProperties()->bUseFixedBrakingDistanceForPaths = false;
	GetCharacterMovement()->GetNavMovementProperties()->FixedPathBrakingDistance = 0;

	CHelpers::CreateActorComponent<UCNoxEnemyHPComponent>(this, &HPComp, "HPComp");
	CHelpers::CreateActorComponent<UCFSMComponent>(this, &FSMComp, "FSMComp");

	GetCapsuleComponent()->SetCollisionProfileName(FName("Enemy"));
}

void ACNox_EBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxAcceleration = AccelValue; // 가속도 설정

	if (auto Anim = GetMesh()->GetAnimInstance())
	{
		EnemyAnim = Cast<UCNoxEnemy_Animinstance>(Anim);
		EnemyAnim->SetEnemy(this);
	}

	if (HPComp) HPComp->SetEnemy(this);

	if (FSMComp) FSMComp->InitializeFSM(this);
}

void ACNox_EBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FSMComp) FSMComp->UpdateState();

	if (FSMComp) // Print Current State
	{
		FString myState = UEnum::GetValueOrBitfieldAsString(FSMComp->GetEnemyState());
		DrawDebugString(GetWorld(), GetActorLocation(), myState, nullptr, FColor::Yellow, 0);
		myState = UEnum::GetValueOrBitfieldAsString(FSMComp->GetCombatState());
		DrawDebugString(
			GetWorld(), FVector(GetActorLocation().X, GetActorLocation().Y,
			                    GetActorLocation().Z - 50), myState, nullptr, FColor::Yellow, 0);
	}
}

void ACNox_EBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	EnemyController = Cast<ACEnemyController>(NewController);
}

void ACNox_EBase::SetApplyDamage(AActor* DamagedPlayer, const float DamageAmout)
{
	UGameplayStatics::ApplyDamage(DamagedPlayer, DamageAmout, EnemyController, this, UDamageType::StaticClass());
}

void ACNox_EBase::SetTarget(ACNox* InTarget)
{
	Target = InTarget;
	Target ? FSMComp->SetEnemyState(EEnemyState::Sense) : FSMComp->SetEnemyState(EEnemyState::IDLE);
}

#pragma region Movement
void ACNox_EBase::SetMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed)
{
	float newSpeed = 0.f, newAccelSpeed = 0.f;
	GetNewMovementSpeed(InMovementSpeed, newSpeed, newAccelSpeed);
	GetCharacterMovement()->MaxWalkSpeed = newSpeed;
	GetCharacterMovement()->MaxAcceleration = newAccelSpeed;
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

void ACNox_EBase::SetTargetCallByDelegate(ACNox* InTarget)
{
	EnemyController->SetTargetPlayer(InTarget);
}

void ACNox_EBase::HandleAttack(float InAttackDistance)
{
	AttackDistance = InAttackDistance;
	EnemyAnim->PlayAttackMontage();
}

bool ACNox_EBase::IsPlayerInDistance()
{
	if (!Target) return false;
	float dist = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
	return dist <= AttackDistance;
}

void ACNox_EBase::HealHP()
{
	HPComp->HealHP(HealAmount);
}

void ACNox_EBase::SetGrenadeEnded(bool InbEndedAnim)
{
	BehaviorComp->SetGrenadeEnded(InbEndedAnim);
}

bool ACNox_EBase::IsPlayerInForwardRange(ACNox* InTarget, float InForwardRange)
{
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * InForwardRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;

	TArray<FHitResult> HitResults;
	bool bHit = GetWorld()->LineTraceMultiByChannel(
		HitResults,
		Start,
		End,
		ECC_Visibility,
		Params
	);
	// DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 0.1f, 0, 2.f);

	for (const FHitResult& Hit : HitResults)
	{
		if (Hit.GetActor())
		{
			// UE_LOG(LogTemp, Log, TEXT("Hit: %s"), *Hit.GetActor()->GetName());

			// 특정 물체에 닿았으면 그 뒤는 검사하지 않음
			if (Hit.GetActor()->ActorHasTag("BlockTrace"))
			{
				break;
			}
			else if (Hit.GetActor() == InTarget)
			{
				return true;
			}
		}
	}

	// CLog::Log(FString::Printf(TEXT("No Hit")));
	return false;
}
