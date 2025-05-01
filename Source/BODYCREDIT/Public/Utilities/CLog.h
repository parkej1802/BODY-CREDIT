#pragma once

#include "CoreMinimal.h"

#define LOCAL_ROLE UEnum::GetValueAsString<ENetRole>(GetLocalRole())
#define REMOTE_ROLE UEnum::GetValueAsString<ENetRole>(GetRemoteRole())

DECLARE_LOG_CATEGORY_EXTERN(NetLog, Log, All);

#define NETMODE (GetWorld()->GetNetMode()==ENetMode::NM_Client ? TEXT("Client") : GetWorld()->GetNetMode()==NM_Standalone ? TEXT("Standalone") : TEXT("Server"))
#define CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")") )
#define PRINTLOG( fmt, ...) UE_LOG(NetLog, Warning, TEXT("[%s]%s : %s"), NETMODE, *CALLINFO, *FString::Printf(fmt, ##__VA_ARGS__))

#define LogLine(){ CLog::Log(__FILE__, __FUNCTION__, __LINE__); }
#define PrintLine(){ CLog::Print(__FILE__, __FUNCTION__, __LINE__); }

class BODYCREDIT_API CLog
{
public:
	static void Log(int32 InValue);
	static void Log(float InValue);
	static void Log(const FString& InValue);
	static void Log(const FVector& InValue);
	static void Log(const FRotator& InValue);
	static void Log(const UObject* InValue);
	static void Log(const FString& InFileName, const FString& InFuncName, int32 InLineNumber);

	static void Print(int32 InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(float InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(const FString& InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(const FVector& InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(const FRotator& InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(const UObject* InValue, int32 InKey = -1, float InDuration = 10, FColor InColor = FColor::Blue);
	static void Print(const FString& InFileName, const FString& InFuncName, int32 InLineNumber);

};
