// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StageDT.generated.h"

USTRUCT(BlueprintType)
struct FStageData: public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float StageDuration = 3.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DayRequireGold = 1000;
};
