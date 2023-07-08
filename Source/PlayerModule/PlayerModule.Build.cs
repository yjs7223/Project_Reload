using UnrealBuildTool;

public class PlayerModule : ModuleRules
{
    public PlayerModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "BaseClassModule", "Niagara", "GameplayCameras", "AIModule" });
    }
}