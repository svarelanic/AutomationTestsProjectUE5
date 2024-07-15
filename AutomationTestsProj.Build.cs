// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AutomationTestsProj : ModuleRules
{
	public AutomationTestsProj(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		//Set macro value to 1 when in editor, to use ifendif for compilation
		if (Target.Type == TargetType.Editor)
		{
			PublicDependencyModuleNames.Add("HeadMountedDisplay");
			PublicDefinitions.Add("MY_VR_MACRO=1");
		}
		else if (Target.Type == TargetType.Game)
		{
			PublicDefinitions.Add("MY_VR_MACRO=0");

		}
	}
}
