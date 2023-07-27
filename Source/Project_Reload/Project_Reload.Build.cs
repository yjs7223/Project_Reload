// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Project_Reload : ModuleRules
{
	public Project_Reload(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"HeadMountedDisplay", 
			"BaseClassModule",
            "PlayerModule",
            "AnimationModule",
            "AIClassModule", 
			"MotionWarping"
        });
	}
}
