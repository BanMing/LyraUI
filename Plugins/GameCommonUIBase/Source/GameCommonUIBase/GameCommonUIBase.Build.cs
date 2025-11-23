// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GameCommonUIBase : ModuleRules
{
    public GameCommonUIBase(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        OptimizeCode = CodeOptimization.Never;
        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
			}
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreOnline",
                "GameplayTags",
                "CommonUI",
                "GameplayAbilities",
                "ModularGameplay",
                "EnhancedInput",
                "UMG",
                "DeveloperSettings",
                "CommonInput"
				// ... add other public dependencies that you statically link with here ...
			}
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreOnline",
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "ApplicationCore",
                "InputCore",
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
