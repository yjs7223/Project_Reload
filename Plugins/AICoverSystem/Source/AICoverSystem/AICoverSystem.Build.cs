//// Copyright, (c) Sami Kangasmaa 2022

using UnrealBuildTool;
using System.IO;

public class AICoverSystem : ModuleRules
{
	public AICoverSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] 
			{
			}
			);

		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/AI"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/AI/Services"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/AI/Tasks"));
		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Public/AI/Tests"));

		PrivateIncludePaths.AddRange(
			new string[] 
			{
				"ThirdParty"
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"NavigationSystem",
				"Navmesh"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "AIModule",
                "GameplayTasks",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
