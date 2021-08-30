// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Grappling_Hook : ModuleRules
{
	public Grappling_Hook(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "GameplayTasks", "Niagara" });

		PublicIncludePaths.AddRange(new string[]{"Grappline_Hook/",
            "C:/Program Files/Epic Games/UE_4.26/Engine/Plugins/FX/Niagara/Source/Niagara/Public",
			"C:/Program Files/Epic Games/UE_4.26/Engine/Plugins/FX/Niagara/Source/Niagara/Private",
			"C:/Program Files/Epic Games/UE_4.26/Engine/Plugins/FX/Niagara/Source/Niagara/Classes"});
	}
}
