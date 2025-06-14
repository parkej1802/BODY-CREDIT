﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/CMemoryData.h"
#include "CMainGM.generated.h"

UCLASS()
class BODYCREDIT_API ACMainGM : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACMainGM();

	int32 ItemIndex = 0;

	int32 GetItemIndex();

private:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ACNox_MemoryCollectorAI* MemoryCollectorAI;

public:
	void RegisterMemoryFromPlayer(class ACNox_Runner* Player, EMemoryTriggerType Trigger);

private:
	const FString VIPZoneID = "VIP";
	UPROPERTY()
	TArray<class ACAreaTriggerBox*> ZoneVolumes;

	FName GetZoneID(class ACNox_Runner* Player);
	// bool IsInVIPZone(class ACNox_Runner* Player);
	bool IsInVIPZone(const FName& ZoneID);
};
