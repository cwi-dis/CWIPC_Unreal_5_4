// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CWIPC_Unreal_5_4 : ModuleRules
{
	public CWIPC_Unreal_5_4(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
