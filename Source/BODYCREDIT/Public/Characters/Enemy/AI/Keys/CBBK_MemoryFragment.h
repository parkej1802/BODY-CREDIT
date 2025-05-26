// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "Data/CMemoryData.h"
#include "CBBK_MemoryFragment.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Memory Fragment"))
class BODYCREDIT_API UCBBK_MemoryFragment : public UBlackboardKeyType
{
	GENERATED_BODY()
	
public:
	typedef FMemoryFragment FValueType;

	static const FValueType& GetValue(const uint8* RawData)
	{
		return *(const FValueType*)RawData;
	}

	static void SetValue(uint8* RawData, const FValueType& Value)
	{
		*(FValueType*)RawData = Value;
	}
};
