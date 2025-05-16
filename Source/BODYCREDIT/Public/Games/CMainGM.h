﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CMainGM.generated.h"

UCLASS()
class BODYCREDIT_API ACMainGM : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACMainGM();

	int32 ItemIndex = 0;

	int32 GetItemIndex();
};
