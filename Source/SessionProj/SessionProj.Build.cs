// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SessionProj : ModuleRules
{
	public SessionProj(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemSteam" });

		DynamicallyLoadedModuleNames.AddRange(new string[] { "OnlineSubsystemNull" });
	}
}
