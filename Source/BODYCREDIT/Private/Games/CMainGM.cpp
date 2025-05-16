#include "Games/CMainGM.h"
#include "Global.h"

ACMainGM::ACMainGM()
{
	ConstructorHelpers::FClassFinder<APawn> pawn(TEXT("/Script/Engine.Blueprint'/Game/Characters/Runner/BP_CNox_Runner.BP_CNox_Runner_C'"));
	if (pawn.Succeeded())
		DefaultPawnClass = pawn.Class;

}

int32 ACMainGM::GetItemIndex()
{
	return ++ItemIndex;
}
