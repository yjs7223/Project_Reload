// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Project_ReloadTarget : TargetRules
{
	public Project_ReloadTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("Project_Reload");
        ExtraModuleNames.Add("BaseClassModule");
        ExtraModuleNames.Add("PlayerModule");
        ExtraModuleNames.Add("AnimationModule");
        ExtraModuleNames.Add("AIClassModule");
        ExtraModuleNames.Add("BossModule");
    }
}
