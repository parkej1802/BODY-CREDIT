#include  "State/CCTV/CRotateMoveStrategy.h"

#include "Characters/Enemy/CNox_CCTV.h"

void CRotateMoveStrategy::Move(ACNox_EBase* Owner, float DeltaTime)
{
	Cast<ACNox_CCTV>(Owner)->RotateCCTV(DeltaTime);
}
