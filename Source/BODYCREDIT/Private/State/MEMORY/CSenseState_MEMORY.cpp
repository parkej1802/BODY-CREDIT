#include "State/MEMORY/CSenseState_MEMORY.h"

#include "Characters/Enemy/CNox_EBase.h"
#include "State/MEMORY/CConditionalMoveStrategy_MEMORY.h"

CSenseState_MEMORY::CSenseState_MEMORY(TUniquePtr<CConditionalMoveStrategy_MEMORY> InMoveStrategy)
	: MoveStrategy(MoveTemp(InMoveStrategy))
{
}

void CSenseState_MEMORY::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	if (MoveStrategy) MoveStrategy->Move(Owner, Owner->GetWorld()->GetDeltaSeconds());

	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Ranged, Owner->GetWorld()->GetDeltaSeconds());
	Owner->UpdateSkillCoolDowns(ESkillCoolDown::Beam, Owner->GetWorld()->GetDeltaSeconds());
	Owner->UpdateSkillCoolDowns(ESkillCoolDown::WavePulse, Owner->GetWorld()->GetDeltaSeconds());
}

void CSenseState_MEMORY::ResetVal(ACNox_EBase* Owner)
{
	if (MoveStrategy) MoveStrategy->ResetVal(Owner);
}
