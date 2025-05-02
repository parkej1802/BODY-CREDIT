#include "DebugPlugIn.h"

#include "Global.h"
#include "CDebuggerCategory.h"

#include "GameplayDebugger.h"

#define LOCTEXT_NAMESPACE "FDebugPlugInModule"
IMPLEMENT_MODULE(FDebugPlugInModule, DebugPlugIn)

void FDebugPlugInModule::StartupModule()
{
	// CLog::Log(TEXT("Start"));
	// IGameplayDebugger& debugger = IGameplayDebugger::Get();	// Singleton화 되있는 것을 가져올때 사용
	
	// IGameplayDebugger::FOnGetCategory category =
	//				IGameplayDebugger::FOnGetCategory::CreateStatic(&FCDebuggerCategory::MakeInstance);
	IGameplayDebugger::FOnGetCategory category;
	category.BindStatic(&FCDebuggerCategory::MakeInstance);
	
	IGameplayDebugger::Get().RegisterCategory("DebugCategory", category,
		EGameplayDebuggerCategoryState::EnabledInGameAndSimulate, 5);

	IGameplayDebugger::Get().NotifyCategoriesChanged();
}

void FDebugPlugInModule::ShutdownModule()
{
	// CLog::Log(TEXT("End"));
	if (IGameplayDebugger::IsAvailable())
		IGameplayDebugger::Get().UnregisterCategory("DebugCategory");
}

#undef LOCTEXT_NAMESPACE
