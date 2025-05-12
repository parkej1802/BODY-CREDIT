#include "Games/CMainGM.h"

#include "EngineUtils.h"
#include "Global.h"
#include "Characters/Enemy/CNox_MemoryCollectorAI.h"
#include "Trigger/CAreaTriggerBox.h"

ACMainGM::ACMainGM()
{
	ConstructorHelpers::FClassFinder<APawn> pawn(
		TEXT("/Script/Engine.Blueprint'/Game/Characters/Runner/BP_CNox_Runner.BP_CNox_Runner_C'"));
	if (pawn.Succeeded())
		DefaultPawnClass = pawn.Class;
}

void ACMainGM::BeginPlay()
{
	Super::BeginPlay();
	for (TActorIterator<ACNox_MemoryCollectorAI> It(GetWorld(), ACNox_MemoryCollectorAI::StaticClass()); It; ++It)
	{
		MemoryCollectorAI = *It;
	}
	for (TActorIterator<ACAreaTriggerBox> It(GetWorld(), ACAreaTriggerBox::StaticClass()); It; ++It)
	{
		ZoneVolumes.Add(*It);
	}
}

void ACMainGM::RegisterMemoryFromPlayer(ACNox_Runner* Player, EMemoryTriggerType Trigger)
{
	if (!HasAuthority() || !Player) return;

	FMemoryFragment NewMemory;
	NewMemory.Location = Player->GetActorLocation();
	NewMemory.TimeStamp = GetWorld()->GetTimeSeconds();
	NewMemory.TriggerType = Trigger;
	NewMemory.SourcePlayer = Player;
	NewMemory.ZoneID = GetZoneID(Player);
	NewMemory.bVIPRelated = IsInVIPZone(NewMemory.ZoneID);

	if (!MemoryCollectorAI) return;
	MemoryCollectorAI->RegisterMemory(NewMemory); // AI에 직접 등록
}

FName ACMainGM::GetZoneID(class ACNox_Runner* Player)
{
	if (!Player) return NAME_None;

	for (const auto& Zone : ZoneVolumes)
	{
		if (Zone && Zone->IsOverlappingActor(Player))
		{
			return Zone->ZoneID; // ex. "VIP_Ward", "EdenWing"
		}
	}
	return NAME_None;
}

bool ACMainGM::IsInVIPZone(const FName& ZoneID)
{
	return ZoneID.ToString().Equals(VIPZoneID);
}

// bool ACMainGM::IsInVIPZone(ACNox_Runner* Player)
// {
// 	if (!Player) return false;
//
// 	// 예시: VIPZoneVolume은 레벨에 배치된 TriggerVolume이나 BoxComponent 기반의 Actor
// 	for (const auto& Zone : ZoneVolumes)
// 	{
// 		if (Zone && Zone->ActorHasTag(FName("VIP")) && Zone->IsOverlappingActor(Player))
// 		{
// 			return true;
// 		}
// 	}
// 	return false;
// }
