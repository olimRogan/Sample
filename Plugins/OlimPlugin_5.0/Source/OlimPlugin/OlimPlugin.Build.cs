// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OlimPlugin : ModuleRules
{
    public OlimPlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        bEnableExceptions = true;
        bLegacyPublicIncludePaths = false;
        //bUsePrecompiled = true;

        PublicIncludePaths.AddRange(new string[] {
            // ... add public include paths required here ...
        });


        PrivateIncludePaths.AddRange(new string[] {
            // ... add other private include paths required here ...
        });

        PublicDependencyModuleNames.AddRange(new string[] {
            "ApplicationCore",
            "Core",
            "RHI",
            "Json",
            "JsonUtilities",
            "HTTP",
			// ... add other public dependencies that you statically link with here ...
            "UMG",
        });

        PrivateDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Slate",
            "SlateCore",
            "RenderCore",
            "RHI",
            // ... add private dependencies that you statically link with here ...	
        });
        
        if (Target.Type == TargetRules.TargetType.Editor)
        {
            PrivateDependencyModuleNames.AddRange(new string[] {
                "UnrealEd",
                "LevelEditor",
                "EditorScriptingUtilities",
            });
        }

        DynamicallyLoadedModuleNames.AddRange(new string[] {
			// ... add any modules that your module loads dynamically here ...
		});

        if (Target.Type != TargetType.Server)
        {
            PrivateIncludePathModuleNames.AddRange(new string[] {
                "SlateRHIRenderer",
            });

            DynamicallyLoadedModuleNames.AddRange(new string[] {
                "ImageWrapper",
                "SlateRHIRenderer",
            });
        }
    }
}
