using UnrealBuildTool;

public class DroneModule : ModuleRules
{
    public DroneModule(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core",
            "CoreUObject", 
            "Engine",  
            "Niagara", 
            "GameplayCameras", 
            "AIModule", 
            "InputCore", 

        });
    }
}