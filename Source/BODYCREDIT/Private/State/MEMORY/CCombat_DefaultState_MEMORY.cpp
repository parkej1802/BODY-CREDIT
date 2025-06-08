#include "State/MEMORY/CCombat_DefaultState_MEMORY.h"

#include "AIController.h"
#include "Characters/Enemy/CNox_EBase.h"
#include "Navigation/PathFollowingComponent.h"

void CCombat_DefaultState_MEMORY::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	AAIController* AICon = Cast<AAIController>(Owner->GetController());
	if (!AICon) return;

	Owner->SetMovementSpeed(EEnemyMovementSpeed::Sprinting);
	EPathFollowingRequestResult::Type result = AICon->MoveToActor(Owner->GetTarget(), AcceptanceThreshold, true);
	if (result == EPathFollowingRequestResult::Failed)
	{
		AICon->StopMovement();
		return;
	}
	else if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		const FRotator TargetRot = (Owner->GetTargetLocation() - Owner->GetActorLocation()).Rotation();
		Owner->SetActorRotation(FMath::RInterpTo(Owner->GetActorRotation(), TargetRot, Owner->GetWorld()->GetDeltaSeconds(), 40.f));
	}
	
	if (!bFired)
	{
		Owner->HandleAttack();
		bFired = true;
	}
	else
	{
		if (!Owner->IsAttacking())
		{
			Owner->UsingSkill(ESkillCoolDown::Ranged);
			Owner->SetEnemyState(EEnemyState::Sense);
			bFired = false;
		}
	}
}
