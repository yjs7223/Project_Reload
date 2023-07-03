using UnrealBuildTool;

public class AIClassModule : ModuleRules
{
    public AIClassModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
<<<<<<< Updated upstream

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "BaseClassModule","PlayerModule","Niagara" });
=======
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "BaseClassModule", "Niagara" , "InputCore", "UMG", "AIModule" });
>>>>>>> Stashed changes
    }
}