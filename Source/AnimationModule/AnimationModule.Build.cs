using UnrealBuildTool;

public class AnimationModule : ModuleRules
{
    public AnimationModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "BaseClassModule" });
    }
}