#include "State/ZERO/CCombat_DefaultState_ZERO.h"

#include "State/ZERO/CDefaultAttackStrategy_ZERO.h"

CCombat_DefaultState_ZERO::CCombat_DefaultState_ZERO(TUniquePtr<class CDefaultAttackStrategy_ZERO> InAttackStrategy)
	: AttackStrategy(MoveTemp(InAttackStrategy))
{
}

void CCombat_DefaultState_ZERO::Execute(class ACNox_EBase* Owner, class UCFSMComponent* FSMComp)
{
	if (AttackStrategy)
		AttackStrategy->Execute(Owner, FSMComp);
}
