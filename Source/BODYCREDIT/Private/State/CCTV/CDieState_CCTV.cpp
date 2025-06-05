#include "State/CCTV/CDieState_CCTV.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/Enemy/CNox_CCTV.h"
#include "Characters/Enemy/CNox_EBase.h"

void CDieState_CCTV::Execute(ACNox_EBase* Owner, UCFSMComponent* FSMComp)
{
	ACNox_CCTV* cctv = Cast<ACNox_CCTV>(Owner);

	if (!bFired)
	{
		if (cctv->DieEffect)
		{
			auto* DieEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				cctv->GetWorld(), cctv->DieEffect, cctv->GetActorLocation(), FRotator::ZeroRotator);
			DieEffect->SetAutoDestroy(true);
		}
		if (cctv->DieSpark)
		{
			FTimerHandle TimerHandle;
			cctv->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [cctv]()
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			   cctv->GetWorld(), cctv->DieSpark, cctv->GetActorLocation(), FRotator::ZeroRotator);
			}, 1.f, false);			
		}
		Owner->HandleDie();
		bFired = true;
	}
}
