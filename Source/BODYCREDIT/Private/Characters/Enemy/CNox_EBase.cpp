#include "Characters/Enemy/CNox_EBase.h"

#include "global.h"
#include "Characters/Enemy/CNoxEnemy_Animinstance.h"
#include "Components/Enemy/CNox_BehaviorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
// #include "BehaviorTree/behaviorTree.h"
#include "Components/Enemy/CFSMComponent.h"
#include "Characters/Enemy/AI/CEnemyController.h"
#include "Components/Enemy/CNoxEnemyHPComponent.h"
#include "Navigation/PathFollowingComponent.h"

ACNox_EBase::ACNox_EBase()
{
	TeamID = 2;

	// CHelpers::CreateActorComponent<UCNox_BehaviorComponent>(this, &BehaviorComp, "Behavior");

	// Controller Setting
	CHelpers::GetClass(&AIControllerClass, TEXT("/Game/Characters/Enemy/AI/BP_NoxController.BP_NoxController_C"));
	// AIControllerClass = ACEnemyController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 목표에 거리가 가까워지면서 속도가 줄어드는 현상 방지
	// GetCharacterMovement()->GetNavMovementProperties()->bUseFixedBrakingDistanceForPaths = false;
	GetCharacterMovement()->GetNavMovementProperties()->FixedPathBrakingDistance = 0;

	// CHelpers::GetAsset(&BehaviorTree,
	//                    TEXT("/Game/Characters/Enemy/AI/BT_Nox.BT_Nox"));
	CHelpers::CreateActorComponent<UCNoxEnemyHPComponent>(this, &HPComp, "HPComp");
	CHelpers::CreateActorComponent<UCFSMComponent>(this, &FSMComp, "FSMComp");

	GetCapsuleComponent()->SetCollisionProfileName(FName("Enemy"));
}

void ACNox_EBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxAcceleration = AccelValue; // 가속도 설정

	// if (bUseBehaviorTree)
	// {
	// 	BehaviorComp->SetEnemyType(EnemyType);
	// }

	if (auto Anim = GetMesh()->GetAnimInstance())
	{
		EnemyAnim = Cast<UCNoxEnemy_Animinstance>(Anim);
		EnemyAnim->SetEnemy(this);
		// if (bUseBehaviorTree)
		// 	EnemyAnim->SetBT(BehaviorComp);
	}

	if (HPComp)
		HPComp->SetEnemy(this);

	if (FSMComp)
		FSMComp->InitializeFSM(this);
}

void ACNox_EBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FSMComp) FSMComp->UpdateState();

	if (FSMComp)
	{
		FString myState = UEnum::GetValueOrBitfieldAsString(FSMComp->GetEnemyState());
		DrawDebugString(GetWorld(), GetActorLocation(), myState, nullptr, FColor::Yellow, 0);
		myState = UEnum::GetValueOrBitfieldAsString(FSMComp->GetCombatState());
		DrawDebugString(
			GetWorld(), FVector(GetActorLocation().X, GetActorLocation().Y,
			                    GetActorLocation().Z - 50), myState, nullptr, FColor::Yellow, 0);
	}

	// if (bAutoMove && Target)
	// {
	// 	float dist = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
	// 	if (dist > MoveDistance)
	// 	{
	// 		// CLog::Log(FString::Printf(TEXT("MoveDistance: %f, dist: %f"), MoveDistance, dist));
	// 		FAIMoveRequest request;
	// 		request.SetGoalActor(Target);
	// 		request.SetAcceptanceRadius(MoveDistance);
	// 		FPathFollowingRequestResult result = EnemyController->MoveTo(request);
	// 		// FVector DirectionVector = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	// 		// AddMovementInput(DirectionVector);
	// 	}
	// }
}

void ACNox_EBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	EnemyController = Cast<ACEnemyController>(NewController);
}

void ACNox_EBase::SetTarget(ACNox* InTarget)
{
	Target = InTarget;
	Target ? FSMComp->SetEnemyState(EEnemyState::Sense) : FSMComp->SetEnemyState(EEnemyState::IDLE);
}

void ACNox_EBase::SetTargetCallByDelegate(ACNox* InTarget)
{
	EnemyController->SetTargetPlayer(InTarget);
}

void ACNox_EBase::HandleAttack(float InAttackDistance)
{
	AttackDistance = InAttackDistance;
	EnemyAnim->PlayAttackMontage();
}

bool ACNox_EBase::IsAttacking()
{
	return EnemyAnim->IsAttacking();
}

bool ACNox_EBase::IsPlayerInDistance()
{
	if (!Target) return false;
	float dist = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
	//CLog::Log(FString::Printf(TEXT("dist <= AttackDistance: %d"), dist <= AttackDistance));
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

void ACNox_EBase::SetMovementSpeed(const EEnemyMovementSpeed& InMovementSpeed)
{
	float newSpeed = 0.f, newAccelSpeed = 0.f;
	GetNewMovementSpeed(InMovementSpeed, newSpeed, newAccelSpeed);
	GetCharacterMovement()->MaxWalkSpeed = newSpeed;
	GetCharacterMovement()->MaxAcceleration = newAccelSpeed;
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
