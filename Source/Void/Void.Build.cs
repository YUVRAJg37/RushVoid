// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Void : ModuleRules
{
	public Void(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput", "NiagaraCore", "Niagara" });
	}
}
