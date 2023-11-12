using UnrealBuildTool;

public class BossModule : ModuleRules
{
    public BossModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "BaseClassModule", "Niagara", "GameplayCameras", "AIModule", "AIClassModule", "InputCore", "UMG" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
    }
}