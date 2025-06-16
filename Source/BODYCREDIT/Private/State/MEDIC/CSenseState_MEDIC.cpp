#include "State/MEDIC/CSenseState_MEDIC.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "State/MEDIC/CConditionalMoveStrategy_MEDIC.h"

CSenseState_MEDIC::CSenseState_MEDIC(TUniquePtr<CConditionalMoveStrategy_MEDIC> InMoveStrategy)
	: MoveStrategy(MoveTemp(InMoveStrategy))
{
}

void CSenseState_MEDIC::Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp)
{
	Owner->PlaySenseSound();
	
	if (MoveStrategy) MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());
	
	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Melee, Owner->GetWorld()->GetDeltaSeconds());
	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Heal, Owner->GetWorld()->GetDeltaSeconds());
	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Grenade, Owner->GetWorld()->GetDeltaSeconds());
}

void CSenseState_MEDIC::ResetVal(ACNox_EBase* Owner)
{
	if (MoveStrategy) MoveStrategy->ResetVal(Owner);
}
