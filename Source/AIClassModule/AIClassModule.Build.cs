using UnrealBuildTool;

public class AIClassModule : ModuleRules
{
    public AIClassModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "BaseClassModule", "Niagara", "AIModule" });
    }
}