using UnrealBuildTool;

public class VehicleModule : ModuleRules
{
    public VehicleModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core",
            "CoreUObject", 
            "Engine", 
            "BaseClassModule", 
            "Niagara", 
            "GameplayCameras", 
            "AIModule", 
            "InputCore", 
            "UMG",
            "UMGEditor",

        });

    }
}