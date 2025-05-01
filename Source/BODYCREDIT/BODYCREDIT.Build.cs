// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BODYCREDIT : ModuleRules
{
	public BODYCREDIT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
            "NavigationSystem",
			"Niagara",
			"UMG",
			"Slate",
			"SlateCore",
            "HTTP",
            "Json",
            "JsonUtilities",
            "OnlineSubsystem",
            "OnlineSubsystemSteam"
        });

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
